//  wordleGuessSecretWord.c
//  Have the program do the guessing to discover the secret wordle word.
//
//  Author: Rafay Khan, 11/21/22
//  System: CLion and XCode
//
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/
/*  Running the program looks like:
        Using file wordsLarge.txt with 12947 words.
        -----------------------------------------------------------

        Enter a secret word or just r to choose one at random: dream
        Trying to find secret word:
               d r e a m

            1. s o r e s
                   * *
            2. s e r e r
                 * *
            3. r a r e r
               * *   *
            4. r a r e e
               * *   *
            5. s a r e e
                 * * *
            6. p a r e r
                 * * *
            7. c a r e r
                 * * *
            8. b a r e r
                 * * *
            9. f a r e r
                 * * *
           10. D a r e r
                 * * *
           11. D e a r e
                 * * *
           12. D e b A r
                 *     *
           13. D R E A r
                       *
           14. D R E A d

           15. D R E A M

        Got it!

     ... and then it runs two more times ...
 */
#include <assert.h> // for assert() sanity checks
#include <ctype.h>  // for toupper()
#include <stdio.h>  // for printf(), scanf()
#include <stdlib.h> // for exit( -1)
#include <string.h> // for strcpy
#include <time.h>   // for time()

// Declare globals
#define WORD_LENGTH                                                            \
  5 // All words have 5 letters, + 1 NULL at the end when stored
#define WORDS_FILE_NAME "wordsLarge.txt"
//#define WORDS_FILE_NAME "wordsTiny.txt"
#define MAX_NUMBER_OF_WORDS                                                    \
  12947         // Number of words in the full set of words file
#define true 1  // Make boolean logic easier to understand
#define false 0 // Make boolean logic easier to understand

typedef struct wordCount wordCountStruct;
struct wordCount {
  char word[WORD_LENGTH + 1]; // The word length plus NULL
  int score;                  // Score for the word
};

//-----------------------------------------------------------------------------------------
// Read in words from file into array.  We've previously read the data file once
// to find out how many words are in the file.
void readWordsFromFile(
    char fileName[], // Filename we'll read from
    wordCountStruct
        *words,     // Array of words where we'll store words we read from file
    int *wordCount) // How many words.  Gets updated here and returned
{
  FILE *inFilePtr = fopen(fileName, "r"); // Connect logical name to filename
  assert(inFilePtr != NULL);              // Ensure file open worked correctly

  // Read each word from file and store into array, initializing the score for
  // that word to 0.
  char inputString[6];
  *wordCount = 0;
  while (fscanf(inFilePtr, "%s", inputString) != EOF) {
    strcpy(words[*wordCount].word, inputString);
    words[*wordCount].score = 0;
    (*wordCount)++;
  }

  // Close the file
  fclose(inFilePtr);
} // end readWordsFromFile(..)

//-----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  Parameters are declared
// to be a generic type, so they will match with anything. This is a two-part
// comparison.  First the scores are compared.  If they are the same, then the
// words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction(const void *a, const void *b) {
  // Before using parameters we have cast them into the actual type they are in
  // our program and then extract the numerical value used in comparison
  int firstScore = ((wordCountStruct *)a)->score;
  int secondScore = ((wordCountStruct *)b)->score;

  // If scores are different, then that's all we need for our comparison.
  if (firstScore != secondScore) {
    // We reverse the values, so the result is in descending vs. the otherwise
    // ascending order return firstScore - secondScore;   // ascending order
    return secondScore - firstScore; // descending order
  } else {
    // Scores are equal, so check words themselves, to put them in alphabetical
    // order
    return strcmp(((wordCountStruct *)a)->word, ((wordCountStruct *)b)->word);
  }
} // end compareFunction(..)

// -----------------------------------------------------------------------------------------
// Find a secret word
void findSecretWord(wordCountStruct allWords[], // Array of all the words
                    int wordCount,     // How many words there are in allWords
                    char secretWord[]) // The word to be guessed
{
  char computerGuess[6]; // Allocate space for the computer guess
  char cpyComputerGuess[6];
  char cpySecretWord[6];
  char cpyOfArr[wordCount][6];
  char emptyLine[6];
  char tempComputerGuess[6];
  char tempSecretWord[6];
  char secCopyGuess[6];
  char firstFive[5][6];

  // had an idea that what if we use some starting words to shorten all attempts
  // and found this gem of a reddit
  // https://www.reddit.com/r/wordle/comments/t28x8u/five_5letter_words_that_all_contain_entirely/

  strcpy(firstFive[0], "chunk");
  strcpy(firstFive[1], "fjord");
  strcpy(firstFive[2], "gymps");
  strcpy(firstFive[3], "vibex");
  strcpy(firstFive[4], "waltz");

  strcpy(emptyLine, "     ");
  for (int i = 0; i <= wordCount; i++) {
    strcpy(cpyOfArr[i], allWords[i].word);
  }
  strcpy(cpySecretWord, secretWord);

  printf("Trying to find secret word: \n");

  printf("       ");
  for (int i = 0; i < WORD_LENGTH; i++) {
    printf("%c ", secretWord[i]);
  }
  printf("\n");
  printf("\n");

  int guessNumber = 1;
  int size_word = 5;
  int size_wordEmpty = 5;
  int foundAt = 0;
  int firstLetter = 0;
  int bestCount = 0;

  while (strcmp(computerGuess, secretWord) != 0) {
    if (bestCount == 0) {
      strcpy(computerGuess, firstFive[bestCount]);
    }
    printf("%5d. ", guessNumber);
    while (computerGuess[0] == '@' || computerGuess[1] == '@' ||
           computerGuess[2] == '@' || computerGuess[3] == '@' ||
           computerGuess[4] == '@' ||
           (strcmp(computerGuess, secCopyGuess) == 0)) {
      bestCount++;
      if (bestCount < 5) {
        strcpy(computerGuess, firstFive[bestCount]);
      } else {
        strcpy(computerGuess, cpyOfArr[foundAt]);
        foundAt++;
      }
    }
    strcpy(cpyComputerGuess, computerGuess);
    strcpy(secCopyGuess, computerGuess);
    for (int i = 0; i < size_word; i++) {
      if (computerGuess[i] != secretWord[i]) {
        printf("%c ", cpyComputerGuess[i]);
        for (int k = 0; k < wordCount; k++) {
          if (cpyOfArr[k][i] == computerGuess[i]) {
            cpyOfArr[k][i] = '@';
          }
        }
      } else {
        for (int j = 0; j < wordCount; j++) {
          if (computerGuess[i] != cpyOfArr[j][i]) {
            cpyOfArr[j][i] = '@';
          }
        }
        printf("%c ", toupper(computerGuess[i]));
        cpyComputerGuess[i] = ' ';
        cpySecretWord[i] = ' ';
      }
    }
    printf("\n");
    strcpy(tempComputerGuess, cpyComputerGuess);
    strcpy(tempSecretWord, cpySecretWord);
    for (int w = 0; w < size_wordEmpty; w++) {
      if (cpyComputerGuess[w] == cpySecretWord[0] &&
          cpyComputerGuess[w] != ' ' && cpySecretWord[0] != ' ') {
        cpySecretWord[0] = ' ';
        cpyComputerGuess[0] = ' ';
        emptyLine[w] = '*';
        for (int d = 0; d < wordCount; d++) {
          if (computerGuess[w] == cpyOfArr[d][w]) {
            cpyOfArr[d][w] = '@';
          }
          for (int g = 0; g < wordCount; g++) {
            if (computerGuess[w] != cpyOfArr[g][0]) {
              cpyOfArr[g][0] = '@';
            }
          }
        }
      }

      else if (cpyComputerGuess[w] == cpySecretWord[1] &&
               cpyComputerGuess[w] != ' ' && cpySecretWord[1] != ' ') {
        cpySecretWord[1] = ' ';
        cpyComputerGuess[1] = ' ';
        emptyLine[w] = '*';
        for (int d = 0; d < wordCount; d++) {
          if (computerGuess[w] == cpyOfArr[d][w]) {
            cpyOfArr[d][w] = '@';
          }
          for (int g = 0; g < wordCount; g++) {
            if (computerGuess[w] != cpyOfArr[g][1]) {
              cpyOfArr[g][1] = '@';
            }
          }
        }
      }

      else if (cpyComputerGuess[w] == cpySecretWord[2] &&
               cpyComputerGuess[w] != ' ' && cpySecretWord[2] != ' ') {
        cpySecretWord[2] = ' ';
        cpyComputerGuess[2] = ' ';
        emptyLine[w] = '*';
        for (int d = 0; d < wordCount; d++) {
          if (computerGuess[w] == cpyOfArr[d][w]) {
            cpyOfArr[d][w] = '@';
          }
          for (int g = 0; g < wordCount; g++) {
            if (computerGuess[w] != cpyOfArr[g][2]) {
              cpyOfArr[g][2] = '@';
            }
          }
        }
      }

      else if (cpyComputerGuess[w] == cpySecretWord[3] &&
               cpyComputerGuess[w] != ' ' && cpySecretWord[3] != ' ') {
        cpySecretWord[3] = ' ';
        cpyComputerGuess[3] = ' ';
        emptyLine[w] = '*';
        for (int d = 0; d < wordCount; d++) {
          if (computerGuess[w] == cpyOfArr[d][w]) {
            cpyOfArr[d][w] = '@';
          }
          for (int g = 0; g < wordCount; g++) {
            if (computerGuess[w] != cpyOfArr[g][3]) {
              cpyOfArr[g][3] = '@';
            }
          }
        }
      }

      else if (cpyComputerGuess[w] == cpySecretWord[4] &&
               cpyComputerGuess[w] != ' ' && cpySecretWord[4] != ' ') {
        cpySecretWord[4] = ' ';
        cpyComputerGuess[4] = ' ';
        emptyLine[w] = '*';
        for (int d = 0; d < wordCount; d++) {
          if (computerGuess[w] == cpyOfArr[d][w]) {
            cpyOfArr[d][w] = '@';
          }
          for (int g = 0; g < wordCount; g++) {
            if (computerGuess[w] != cpyOfArr[g][4]) {
              cpyOfArr[g][4] = '@';
            }
          }
        }
      }
    }
    strcpy(cpyComputerGuess, tempComputerGuess);
    strcpy(cpySecretWord, tempSecretWord);
    printf("       ");
    for (int i = 0; i < 5; i++) {
      printf("%c ", emptyLine[i]);
    }
    strcpy(emptyLine, "     ");

    printf("\n");

    // Update guess number
    guessNumber++;

  } // end for( int i...)
  strcpy(cpyComputerGuess, "     ");
  strcpy(computerGuess, "     ");
  printf("\n Got it! \n");
} // end findSecretWord

// -----------------------------------------------------------------------------------------
int main() {
  char wordsFileName[81]; // Stores the answers file name
  strcpy(wordsFileName,
         WORDS_FILE_NAME); // Set the filename, defined at top of program.
  srand((unsigned)time(
      NULL)); // Seed the random number generator to be current time
  // Declare space for all the words, of a maximum known size.
  wordCountStruct allWords[MAX_NUMBER_OF_WORDS];
  // Start out the wordCount to be the full number of words.  This will decrease
  // as
  //    play progresses each time through the game.
  int wordCount = 0;
  // The secret word that the computer will try to find, plus the return
  // character from fgets.
  char secretWord[WORD_LENGTH + 1];
  char userInput[81]; // Used for menu input of secret word

  // Read in words from file, update wordCount and display information
  readWordsFromFile(wordsFileName, allWords, &wordCount);
  printf("Using file %s with %d words. \n", wordsFileName, wordCount);

  // Run the word-guessing game three times
  for (int i = 0; i < 3; i++) {
    // Reset secret Word
    strcpy(secretWord, "");
    // Display prompt
    printf("-----------------------------------------------------------\n");
    printf("\n");
    printf("Enter a secret word or just r to choose one at random: ");
    scanf(" %s", userInput);
    // Eliminate the return character at end or userInput if it is there
    int length = (int)strlen(userInput);
    if (userInput[length] == '\n') {
      userInput[length] = '\0';
    }
    strcpy(secretWord, userInput); // Store the secret word from user input

    // If input was 'r' then choose a word at random.
    if (strlen(secretWord) <= 1) {
      // Randomly select a secret word to be guessed.
      int randomIndex = rand() % wordCount;
      strcpy(secretWord, allWords[randomIndex].word);
    }

    // Run the game once with the current secret word
    findSecretWord(allWords, wordCount, secretWord);
  }

  printf("Done\n");
  printf("\n");
  return 0;
} // end main()