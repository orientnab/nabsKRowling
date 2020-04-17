#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
// #include <queue>
#include <deque>
#include <iterator>
#include <ctime>
#include <algorithm>

int main(int argc, char *argv[]) {

  if (argc < 3)
      std::cout << "textGenMarkovChain ORDER FILE..." << std::endl;
    return 1;

  // std::string filename = argv[2];
  int markovOrder = std::stoi(argv[1]);
  std::cout << "Markov chain order: " << markovOrder << std::endl;

  std::ifstream myfile;
  std::string myline;
  // Deque helps for debugging, but I could just use queue,
  // which is probably more efficient
  // std::queue<std::string> chain;
  std::deque<std::string> chain;
  std::map<std::deque<std::string>,std::vector<std::string> * > nextState;


  for (int ifile = 2; ifile < argc; ifile++) {
    // Opening
    std::string filename = argv[ifile];
    std::cout << "Reading: " << filename << std::endl;
    myfile.open (filename);
    if (!myfile.is_open()) {
      std::cout << "The file could not be opened." << std::endl;
      return 1;
    }
    // Fill in the first few elements
    chain.clear();
    for (int i = 0; i < markovOrder; i++) {
      getline(myfile,myline,' ');
      std::transform(myline.begin(), myline.end(), myline.begin(), ::tolower);
      std::size_t iend = myline.find_first_of(" \t");
      std::string word = myline.substr(0,iend);
      chain.push_back(word);
    }
    // Fill the map
    while (getline(myfile,myline)) {
      std::transform(myline.begin(), myline.end(), myline.begin(), ::tolower);
      std::string word;
      std::size_t istart = 0;
      std::size_t iend = myline.find_first_of(" \t",istart);
      while (iend < myline.npos) {
        std::string word = myline.substr(istart,iend-istart);
        if(!nextState[chain])
          nextState[chain] = new std::vector<std::string>;
        nextState[chain]->push_back(word);
        chain.push_back(word);
        chain.pop_front();
        // std::cout << chain.size() << std::endl;
        istart = iend+1;
        iend = myline.find_first_of(" \t",istart);
      }
    }
    myfile.close();
    std::cout << "Closing: " << filename << std::endl;
  }

  // for(auto v : nextState) {
  //   for (auto s : v.first) {
  //     std::cout << s << " ";
  //   }
  //   std::cout << " -> ";
  //   for (auto s : *v.second) {
  //     std::cout << s << " ";
  //   }
  //   std::cout << std::endl;
  // }
  int count = 0;
  for(auto v : nextState) {
    count += v.second->size();
  }
  std::cout << count/(nextState.size()*1.0) << std::endl;

// Create new text
  std::string word;
  // std::srand(0); // Constant seed for debugging
  std::srand(std::time(0));
  auto it = nextState.begin();
  int indexInitChain = rand() % nextState.size();
  advance(it, indexInitChain);
  chain = it->first;
  int randint;
  for (int i = 0; i < 500; i++) {
    int a = rand();
    int b = nextState[chain]->size();
    randint = a % b;
    // randint = rand() % nextState[chain]->size();
    word = nextState[chain]->at(randint);
    // std::cout << word << " (" << a << " " << b << " " << randint << ") ";
    std::cout << word << " ";
    chain.push_back(word);
    chain.pop_front();
  }
  std::cout << std::endl;

  return 0;
}
