/********************************************************************
 File name: wordstats.cpp

 Purpose: The source code file wordstats.cpp provides an implementation for the suggested
 public subroutines listed in the supplied wordstats.h header file, and supplements it
 with additional private functions meant to serve certain public functions. The source code
 enables reading the dictionary.txt file to store the known words into a set of strings,
 and additionally reads the testdata.txt file into a map holding a word and the positions it
 occurs at within the file. The program then illustrates splendid STL functionality via
 displaying the statistics on the known and unknown words from the test input, displaying the
 most frequent known and unknown words and ultimately displaying the original text.


 Date: 3 December 2018

 Author: Priya Aswani

 ID Number: 5925502

 Email: pa470@uowmail.edu.au

 ********************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <iterator>
#include "wordstats.h"
using namespace std;

//=================================== Public Functions ===================================//

//default constructor, initialises Filename to "testdata.txt"
WordStats::WordStats() {
	strcpy(Filename, "testdata.txt");
}

//method reads dictionary.txt into Dictionary, a set of strings
void WordStats::ReadDictionary() {
	ifstream inFile;
	inFile.open("dictionary.txt");

	if (!inFile.good()) {
		cerr << "Cannot find file! Program terminating." << endl;
		exit(1);
	}

	string word;
	while (inFile.peek() != EOF) {
		inFile >> word;
		word = processWord(word);
		Dictionary.insert(word);
	}

	inFile.close();
	cout << Dictionary.size() << " words read from dictionary." << endl << endl;

}

//method displays the first twenty words of the set of strings read from dictionary.txt
void WordStats::DisplayDictionary() {
	if (Dictionary.size() == 0) {
		cerr << "Dictionary is empty. Cannot output words. Program terminating."
				<< endl;
		exit(1);
	}

	if (Dictionary.size() < 20) {
		cerr << "Dictionary has less than twenty words. Program terminating."
				<< endl;
		exit(1);
	}

	cout << "Displaying the first twenty words in the Dictionary ... " << endl;
	set<string>::iterator iterator = Dictionary.begin();
	for (int counter = 1; counter <= 20; counter++, iterator++) {
		cout << *iterator << endl;
	}

	cout << endl;

}

//method reads individual words with their respective positions and inserts this into a WordMap
//of either KnownWords or UnknownWords depending upon whether the word exists in the
//dictionary or otherwise
void WordStats::ReadTxtFile() {
	ifstream inFile;
	inFile.open(Filename);
	if (!inFile.good()) {
		cerr << "Cannot find file! Program terminating." << endl;
		exit(1);
	}

	int position;
	string word;

	while (inFile.peek() != EOF) {
		position = 0;
		vector<int> positions;
		inFile >> word;
		word = processWord(word);

		ifstream inFile2;
		inFile2.open(Filename);
		if (!inFile2.good()) {
			cerr << "Cannot find file! Program terminating." << endl;
			exit(1);
		}

		while (inFile2.peek() != EOF) {
			string tempWord;
			inFile2 >> tempWord;
			++position;
			tempWord = processWord(tempWord);
			if (tempWord == word) {
				positions.push_back(position);
			}
		}

		inFile2.close();
		inFile2.clear();

		set<string>::iterator finder = Dictionary.find(word);
		if (finder != Dictionary.end()) {
			KnownWords[word] = positions;
		} else {
			UnknownWords[word] = positions;
		}

	}

	cout << KnownWords.size() << " known words read." << endl;
	cout << UnknownWords.size() << " unknown words read." << endl;
	cout << endl;
}

//method displays statistics of known words i.e., the word itself, its frequency of occurrence
//and the positions it occurs at in testdata.txt. The method utilises a private function
//printWordMap() to achieve this for the WordMap of known words
void WordStats::DisplayKnownWordStats() {
	printWordMap(KnownWords);
}

//method displays statistics of unknown words i.e., the word itself, its frequency of occurrence
//and the positions it occurs at in testdata.txt. The method utilises a private function
//printWordMap() to achieve this for the WordMap of unknown words
void WordStats::DisplayUnknownWordStats() {
	printWordMap(UnknownWords);
}

//method displays the 10 most frequent known words and the pertinent frequencies
//The method utilises a private function mostFrequentWords() to achieve this for the WordMap
//of known words
void WordStats::DisplayMostFreqKnownWords() {
	mostFrequentWords(KnownWords);

}

//method displays the 10 most frequent unknown words and the pertinent frequencies
//The method utilises a private function mostFrequentWords() to achieve this for the WordMap
//of unknown words
void WordStats::DisplayMostFreqUnknownWords() {
	mostFrequentWords(UnknownWords);
}

//method reconstructs the original text from the WordMaps of known and unknown words
//and then displays it. The method makes use of a map of position of occurrence and the word
void WordStats::DisplayOriginalText() {
	map<int, string> originalText;
	WordMapIter knownWordsIterator = KnownWords.begin();
	int position;
	string word;
	vector<int> positions;
	while (knownWordsIterator != KnownWords.end()) {
		word = knownWordsIterator->first;
		positions = knownWordsIterator->second;
		for (unsigned int counter = 0; counter < positions.size(); counter++) {
			position = positions[counter];
			originalText.insert(pair<int, string>(position, word));
		}
		knownWordsIterator++;
	}

	WordMapIter unknownWordsIterator = UnknownWords.begin();

	while (unknownWordsIterator != UnknownWords.end()) {
		word = unknownWordsIterator->first;
		positions = unknownWordsIterator->second;
		for (unsigned int counter = 0; counter < positions.size(); counter++) {
			position = positions[counter];
			originalText.insert(pair<int, string>(position, word));
		}
		unknownWordsIterator++;
	}

	map<int, string>::iterator originalTextIterator = originalText.begin();
	while (originalTextIterator != originalText.end()) {

		cout << originalTextIterator->second << " " << flush;
		originalTextIterator++;

	}
}

//=================================== Private Functions ===================================//

//method processes a word so as to convert it into lowercase and remove all non-alphabetic
//characters with the exception of punctuation '(apostrophe) and -(hyphen).
//If the string is numeric then no such processing is done and the original numeric string
//is returned.
string WordStats::processWord(string word) {
	transform(word.begin(), word.end(), word.begin(), ::tolower);
	bool isNumber = true;
	string::iterator wordIterator = word.begin();
	while (wordIterator != word.end()) {

		if (!isdigit(*wordIterator)) {
			isNumber = false;
			break;
		}
		wordIterator++;

	}
	if (!isNumber) {
		word.erase(
				remove_if(word.begin(), word.end(),
						[](char c) {return !isalpha(c)&& c!='-' && c!='\'';}),
				word.end());
	}

	return word;
}

//method to print the contents of the WordMap passed as an argument. The details printed
//include the word itself, its frequency of occurrence and the positions at which it occurred
//in testdata.txt
void WordStats::printWordMap(WordMap& wordMap) {
	cout << right;
	cout << setw(15) << "Word" << setw(14) << "Count\t" << left
			<< "  Position(s)" << endl;
	WordMapIter wordMapIterator = wordMap.begin();
	while (wordMapIterator != wordMap.end()) {
		cout << right << setw(15) << wordMapIterator->first << setw(11)
				<< wordMapIterator->second.size() << flush;
		vector<int> positions = wordMapIterator->second;
		cout << "\t  " << left;
		for (unsigned int i = 0; i < positions.size(); i++) {
			cout << positions[i] << " " << flush;
		}
		cout << endl;
		wordMapIterator++;
	}

	cout << endl;

}

//method to print the 10 most frequent words of the WordMap passed as argument.
//The method develops a multimap of frequency of occurrence and the word and displays
//details of words with frequencies falling in the top ten highest frequencies
void WordStats::mostFrequentWords(WordMap& wordMap) {

	multimap<int, string, greater<int> > wordsFrequency;
	WordMapIter wordMapIterator = wordMap.begin();
	string word;
	int frequency;
	while (wordMapIterator != wordMap.end()) {

		word = wordMapIterator->first;
		frequency = (wordMapIterator->second).size();
		wordsFrequency.insert(pair<int, string>(frequency, word));
		wordMapIterator++;
	}

	multimap<int, string>::iterator wordFrequencyIterator =
			wordsFrequency.begin();
	cout << right << setw(13) << "Frequency" << left << "\tWord" << endl;
	for (int counter = 1; counter <= 10; counter++) {

		cout << right << setw(13) << wordFrequencyIterator->first << left
				<< "\t" << wordFrequencyIterator->second << endl;
		wordFrequencyIterator++;

	}

	cout << endl;

}

