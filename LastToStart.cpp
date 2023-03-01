#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

//My modified insertionSort
vector<int> insertionSort(vector<int> rawData, int x, vector<int>& job, vector<int>& end);

//The actual algorithm that finds which jobs can be assigned, starting from the last job's start time.
void LastToStart(vector<int> start, vector<int> end, vector<int> job);


//The main goal of this function is to sort the start times in ascending order. 
vector<int> insertionSort(vector<int> rawData, int x, vector<int>& job, vector<int>& end) {
	//Keeping track of some indexes for each vector, as well as a temp int for the current index
	int ptr;
	int indexStart;
	int indexEnd;
	int indexJob;

	vector<int> returnVector;
	
	//Keep track of the first compared value for each vector
	for (int i = 1; i < x; i++) {
		indexStart = rawData[i];
		indexEnd = end[i];
		indexJob = job[i];
		ptr = i - 1;
		//If the startTime vector does end up needing to be sorted, I want to also move each index of the endTime and JobID vectors to the same
		//index as the startTime vector.
		while (ptr >= 0 && rawData[ptr] > indexStart) {
			rawData[ptr + 1] = rawData[ptr];
			job[ptr + 1] = job[ptr];
			end[ptr + 1] = end[ptr];
			ptr--;
		}
		//After the sorting, put the original compared value where it belongs in each vector
		rawData[ptr + 1] = indexStart;
		end[ptr + 1] = indexEnd;
		job[ptr + 1] = indexJob;
	}

	//Now I'm going to take everything in the that was sorted for the startTime vector and put it into a
	//new vector so I can return it.
	for (int i = 0; i < rawData.size(); i++) {
		returnVector.push_back(rawData[i]);
	}

	return returnVector;

}

void LastToStart(vector<int> start, vector<int> end, vector<int> job) {
	vector<int> activities;
	int numActivities = 1;
	int currentStartTime;
	
	//We know that the last job to start will always be included, so I go ahead and push that to the activities vector
	activities.push_back(job[start.size() - 1]);

	//I also want to keep track of this index's start time so that I can compare it to other end times
	currentStartTime = start[start.size() - 1];		 

	//Now, excluding the last time to start, I want to loop through the remaining jobs,
	//starting from the top (since they are in ascending order), and find a end time that is
	// less than or equal to the current start time. 
	for (int i = start.size() - 2; i > -1; i--) {
		if (end[i] <= currentStartTime) {
			activities.push_back(job[i]);	//Push it to the activities vector
			currentStartTime = start[i];	//Adjust the new current startTime
			numActivities++;				//Increment the number of activities
		}
	}

	// Duplicating the results. 
	cout << "Number of Activities: " << numActivities << endl;
	cout << "Activities: ";
	for (int i = activities.size()-1; i > -1; i--) {
		cout << activities[i] << " ";
	}
	cout << endl;

}

int main() {

	int setNumber = 1;		//Set number will always start at 1 unless there are no jobs
	int temp = 0;
	int numJobs = 0;

	vector<int> startTime;
	vector<int> endTime;
	vector<int> jobID;


	ifstream dataStream;		//Getting data from the text file

	dataStream.open("act.txt");
	
	while (!dataStream.eof()) {
		dataStream >> numJobs;
		for (int i = 0; i < numJobs; i++) {
			dataStream >> temp;
			jobID.push_back(temp);
			dataStream >> temp;
			startTime.push_back(temp);
			dataStream >> temp;
			endTime.push_back(temp);
		}
		//For every set of jobs, we want to perform these functions
		startTime = insertionSort(startTime, startTime.size(), jobID, endTime); //First sort the start times.
		cout << "Set Number: " << setNumber << endl;
		LastToStart(startTime, endTime, jobID); //Now find which jobs will work
		setNumber++;	//Increment set Number for the next set.
		cout << endl;

		//Clearing my vectors so I can use them again for the next set.
		startTime.clear();
		endTime.clear();
		jobID.clear();
	}
	return 0;
}