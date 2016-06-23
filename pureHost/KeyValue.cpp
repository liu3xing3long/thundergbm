/*
 * KeyValue.cpp
 *
 *  Created on: 4 May 2016
 *      Author: Zeyi Wen
 *		@brief: 
 */

#include <algorithm>

#include "KeyValue.h"
#include "MyAssert.h"

using std::sort;

/**
 * @brief: sort a vector in a descendant order
 */
bool CmpValue(const KeyValue &a, const KeyValue &b)
{
  return a.featureValue > b.featureValue;
}

/**
 * @brief: (1) for each feature, sort the key value ordering based on the value; (2) store the sorted results to vectors
 */
void KeyValue::SortFeaValue(int nNumofDim, vector<vector<KeyValue> > &vvInsSparse, vector<vector<KeyValue> > &vvFeaInxPair)
{
	//sort the feature values for each feature
	vector<int> vCurParsePos;
	int nNumofIns = vvInsSparse.size();
	for(int i = 0; i < nNumofIns; i++)
	{
		vCurParsePos.push_back(0);
	}

	for(int j = 0; j < nNumofDim; j++)
	{
		vector<KeyValue> featurePair;
		for(int i = 0; i < nNumofIns; i++)
		{
			int curTop = vCurParsePos[i];
			if(vvInsSparse[i].size() == curTop)
				continue;

			int curFeaId = vvInsSparse[i][curTop].id;
			if(curFeaId == j)
			{
				KeyValue kv;
				kv.id = i;
				kv.featureValue = vvInsSparse[i][curTop].featureValue;
				featurePair.push_back(kv);
				vCurParsePos[i] = vCurParsePos[i] + 1;
			}
		}

		sort(featurePair.begin(), featurePair.end(), CmpValue);

		vvFeaInxPair.push_back(featurePair);
	}
}

/**
 * @brief: convert a vector to an array
 * @vvFeaInxPair: each feature has a vector of key (which is instance id) and value
 * @pId:  all the instance/feature ids corresponding pair
 * @pdValue: all the feature values
 * @pNumofKeyValue: number of key-value pairs of each feature
 */
void KeyValue::VecToArray(vector<vector<KeyValue> > &vvKeyValuePair, int *pId, double *pdValue, int *pNumofKeyValue, long long *plStartPos)
{
	PROCESS_ERROR(pId != NULL);
	PROCESS_ERROR(pdValue != NULL);
	PROCESS_ERROR(pNumofKeyValue != NULL);

	int nCur = 0;//the current processing key-value pair
	int nKeyValue = vvKeyValuePair.size();
	long long totalPreValue = 0;
	for(int i = 0; i < nKeyValue; i++)
	{//for each feature
		vector<KeyValue> &vKV = vvKeyValuePair[i];
		int nNumofPair = vKV.size();
		pNumofKeyValue[i] = nNumofPair;

		//for each pair of key-value
		for(int p = 0; p < nNumofPair; p++)
		{
			pId[nCur] = vKV[p].id;
			pdValue[nCur] = vKV[p].featureValue;
			nCur++;//move to next key value
		}

		plStartPos[i] = totalPreValue;
		totalPreValue += nNumofPair;
	}
}

/**
 * @brief: a function to test VecToArry
 */
void KeyValue::TestVecToArray(vector<vector<KeyValue> > &vvFeaInxPair, int *pInsId, double *pdValue, int *pNumofKeyValue)
{
	PROCESS_ERROR(pInsId != NULL);
	PROCESS_ERROR(pdValue != NULL);
	PROCESS_ERROR(pNumofKeyValue != NULL);

	int nCur = 0;//the current processing key-value pair
	int nFeature = vvFeaInxPair.size();
	for(int i = 0; i < nFeature; i++)
	{//for each feature
		vector<KeyValue> &vKV = vvFeaInxPair[i];
		int nNumofValue = vKV.size();
		PROCESS_ERROR(pNumofKeyValue[i] == nNumofValue);

		//for each pair of key-value
		for(int p = 0; p < nNumofValue; p++)
		{
			PROCESS_ERROR(pInsId[nCur] == vKV[p].id);
			PROCESS_ERROR(pdValue[nCur] == vKV[p].featureValue);
			nCur++;//move to next key value
		}
	}
}