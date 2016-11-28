#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct date {
  int day;
  enum month {
      january, february, march, april, may, june,
      july, august, september, october, november, december
  } month;
  int year;
} date;

typedef struct tiime {
  int hour;
  int minute;
} tiime;

typedef struct game {
  int round;
  date gameDate;
  char dayName[4];
  tiime gameTime;
  char homeTeam[4];
  char awayTeam[4];
  int homeGoal;
  int awayGoal;
  int spec; //spectators
} game;

typedef struct team {
  char name[4];
  int point;
  int gamePlayed;
  int gameWin;
  int gameWinAway;
  int gameWinHome;
  int gameLost;
  int gameTied;
  int goalScored;
  int goalAgainst;
  int specTotal;
} team;

typedef struct round {
  int number;
  int goalTotal;
} round;

game *readGames(int totalGames);
int countLinesInFile(char *);
date stringToDateFormat(const char *);
tiime stringToTimeFormat(const char *);
int stringToSpecFormat(const char *);
game createGame(int, char[4], date, tiime, char[4], char[4],
              int, int, int);

int main ()
{
  int i;
  int totalGames = countLinesInFile("superliga-2015-2016");

  game *games = readGames(totalGames);

  // printf("Runde nr. %d\n", games->round);
  // printf("Kamp nr. 1, hjemmehold: %s\n", games->homeTeam);

  return(0);
}

game *readGames(int totalGames)
{
  FILE *file = fopen("superliga-2015-2016", "r");

  game *games = (game *) malloc(sizeof(game) * totalGames);

  int i, round, homeGoal, awayGoal, spec;
  date date;
  tiime tiime;
  char dayName[4], homeTeam[4], awayTeam[4],
    date_string[15], time_string[15], spec_string[15];

  for (i = 0; i < totalGames; ++i) {
    fscanf(file, " R%d %s %s %s %s - %s %d - %d %s ",
      &round, dayName, date_string, time_string, homeTeam, awayTeam,
      &homeGoal, &awayGoal, spec_string);

    date = stringToDateFormat(date_string);
    tiime = stringToTimeFormat(time_string);
    spec = stringToSpecFormat(spec_string);

    games[i] = createGame(round, dayName, date, tiime, homeTeam, awayTeam, homeGoal, awayGoal, spec);
  }

  fclose(file);

  return games;
}

int countLinesInFile(char *file)
{
  char line[100];
  int res = 0;
  FILE *in = fopen(file, "r");
  if (in == NULL) {
      printf("fejl i åbning af filen: %s\n", file);
      exit(EXIT_FAILURE);
  }

  while (fgets(line, 100, in) != NULL
         && strcmp(line, "") != 0) {
      res++;
  }

  fclose(in);
  return res;
}

date stringToDateFormat(const char *date_string)
{
  date newDate;

  int day, month, year;

  sscanf(date_string, "%d/%d/%d", &day, &month, &year);

  newDate.day = day;
  newDate.month = (enum month) (month - 1);
  newDate.year = year;

  return newDate;
}

tiime stringToTimeFormat(const char *time_string)
{
  tiime newTime;

  int hour, minute;

  sscanf(time_string, "%d.%d", &hour, &minute);

  newTime.hour = hour;
  newTime.minute = minute;

  return newTime;
}

int stringToSpecFormat(const char *spec_string)
{
    int first, second;
    int scanRes = sscanf(spec_string, "%d.%d", &first, &second);
    if (scanRes == 2) {
        return first * 1000 + second;
    } else if (scanRes == 1) {
        return first;
    } else {
        printf("Der opstod en fejl under indlæsning af tilskuertal: %s\n", spec_string);
        exit(EXIT_FAILURE);
    }
}

game createGame(int round, char dayName[4], date gameDate, tiime gameTime,
              char homeTeam[4], char awayTeam[4],
              int homeGoal, int awayGoal, int spec)
{
    game game = { round, "", gameDate, gameTime, "", "", homeGoal, awayGoal, spec };
    strcpy(game.dayName, dayName);
    strcpy(game.homeTeam, homeTeam);
    strcpy(game.awayTeam, awayTeam);
    return game;
}