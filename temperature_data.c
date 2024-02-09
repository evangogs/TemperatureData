/* temperature_data.c
Evan Goguillot V01013624
Date code was implemented: 2022-10-30

   B. Bird - 2022-11-19
*/

#include <stdio.h>
#include "temperature_data.h"

/* Do not copy the typedef statements for the structure types into this file
   (they should only be in the .h file)
*/

/* read_observation(input_file, obs)
   Given a file object input_file and a pointer obs to an Observation
   structure, try to read a single observation from the file into the structure
   pointed to by obs. If the complete observation is read successfully, return 1.
   Otherwise, return 0.

   Parameters: input_file (FILE pointer), obs (pointer to Observation object)
   Return value (integer):
     - If the observation was read successfully, return 1
     - Otherwise, return 0
*/
int read_observation(FILE *input_file, Observation *obs)
{

    int total;

    total = fscanf(input_file, "%d %d %d %d %d %d %f", &(obs->obs_date.year), &(obs->obs_date.month),
                   &(obs->obs_date.day), &(obs->hour), &(obs->minute), &(obs->station_id),
                   &(obs->temperature));
    if (total != 7)
    {
        total = 0;
    }
    else
    {
        total = 1;
    }
    return total;
}

/* count_observations(filename)
   Given a filename, count and return the number of temperature observations in
   the file. If the file cannot be opened, return -1.

   Parameters: filename (string)
   Return value (integer):
     - If the file cannot be opened for reading, return -1
     - Otherwise, return the number of temperature observations in the file.
*/
int count_observations(char filename[])
{

    FILE *obs = fopen(filename, "r");

    if (obs == NULL)
    {
        return -1;
    }

    int scanner;
    int counter = 0;
    float j = 0;

    int x = (fscanf(obs, "%d %d %d %d %d %d %f", &scanner, &scanner, &scanner, &scanner, &scanner, &scanner, &j));

    while (x == 7)
    {

        x = fscanf(obs, "%d %d %d %d %d %d %f", &scanner, &scanner, &scanner, &scanner, &scanner, &scanner, &j);
        counter++;
    }

    fclose(obs);

    return counter;
}

/* load_all_observations(filename, array_size, observation_array)
   Given a filename and an array of Observation objects, read as many
   observations from the file as possible (up to the size of the array)
   and store each observation (in order) into the array.

   If the file cannot be opened, return -1. Otherwise, return the number of
   observations read.

   If the file contains fewer observations then there are elements in the array,
   stop reading after the last observation in the file. Otherwise, stop reading
   once array_size observations are read.

   Parameters: filename (string), array_size (integer), observation_array (array of Observation)
   Return value (integer):
     - If the file could not be opened, return -1
     - Otherwise, return the total number of observations read and stored into
       the array.
*/
int load_all_observations(char filename[], int array_size, Observation observation_array[array_size])
{

    FILE *obs = fopen(filename, "r");

    if (obs == NULL)
    {
        return -1;
    }

    int wc = 0;

    while (wc < array_size)
    {

        fscanf(obs, "%d %d %d %d %d %d %f", &(observation_array[wc].obs_date.year), &(observation_array[wc].obs_date.month),
               &(observation_array[wc].obs_date.day), &(observation_array[wc].hour),
               &(observation_array[wc].minute), &(observation_array[wc].station_id),
               &(observation_array[wc].temperature));
        wc++;
    }

    fclose(obs);

    return wc;
}

/* print_station_extremes(num_observations, obs_array)
   Given an array of Observation objects, compute and print the
    _extreme observations_ (lowest temperature observed and highest
   temperature observed) for each station that has at least one
   observation in the dataset.

   The output will contain one line for each station with at least one
   observation in the dataset, using a format equivalent to the following:
      Station 1: Minimum = -1.87 degrees (2022-11-21 06:10), Maximum = 10.6 degrees (2022-01-11 01:16)

   You should use the following printf format string to achieve the correct
   output format.
      "Station %d: Minimum = %.2f degrees (%04d-%02d-%02d %02d:%02d), Maximum = %.2f degrees (%04d-%02d-%02d %02d:%02d)\n"

   The output must be ordered by station number (with lower numbered station
   numbers appearing first). No more than one line of output should be
   generated for a particular station number.

   In cases where multiple observations achieve the extreme value (e.g. if the
   minimum temperature at a particular station is -1.87, but there are several
   observations with this temperature), print the date/time of the
   chronologically earliest observation with that extreme temperature.

   You may assume that all observations contain a station number between
   1 and 250 (inclusive).

   This function must _not_ use any file I/O features whatsoever.

   Parameters: num_observations (integer), observation_array (array of Observation)
   Return value: None
   Side Effect: A printed representation of station extremes is output to the user.
*/
void print_station_extremes(int num_observations, Observation obs_array[num_observations])
{

    int statnum = 251;   // obs_array[].station_id
    int statnumlast = 0; // the last lowest station number;
    float maxtemp;       // obs_array[].temperature
    float mintemp;
    int maxyear;   // obs_array[].obs_date.year
    int maxmonth;  // obs_array[].obs_date.month
    int maxday;    // obs_array[].obs_date.day
    int maxhour;   // obs_array[].hour
    int maxminute; // obs_array[].minute
    int minyear;   // obs_array[].obs_date.year
    int minmonth;  // obs_array[].obs_date.month
    int minday;    // obs_array[].obs_date.day
    int minhour;   // obs_array[].hour
    int minminute;

    for (int i = 0; i < num_observations; i++)
    {
        maxtemp = -100;
        mintemp = 100;

        for (int j = 0; j < num_observations; j++)
        {
            if (statnum > obs_array[j].station_id)
            {
                if (statnumlast < obs_array[j].station_id)
                {
                    statnum = obs_array[j].station_id;
                }
            }
        }

        statnumlast = statnum;

        for (int e = 0; e < num_observations; e++)
        {

            if (obs_array[e].station_id == statnum)
            {

                if (obs_array[e].temperature == maxtemp)
                {
                    if (obs_array[e].obs_date.year == maxyear)
                    {
                        if (obs_array[e].obs_date.month == maxmonth)
                        {
                            if (obs_array[e].obs_date.day == maxday)
                            {
                                if (obs_array[e].hour == maxhour)
                                {
                                    if (maxminute > obs_array[e].minute)
                                    {

                                        maxminute = obs_array[e].minute;
                                    }
                                }
                                else if (maxhour > obs_array[e].hour)
                                {
                                    maxhour = obs_array[e].hour;
                                    maxminute = obs_array[e].minute;
                                }
                            }
                            else if (maxday > obs_array[e].obs_date.day)
                            {
                                maxday = obs_array[e].obs_date.day;
                                maxhour = obs_array[e].hour;
                                maxminute = obs_array[e].minute;
                            }
                        }
                        else if (maxmonth > obs_array[e].obs_date.month)
                        {
                            maxmonth = obs_array[e].obs_date.month;
                            maxday = obs_array[e].obs_date.day;
                            maxhour = obs_array[e].hour;
                            maxminute = obs_array[e].minute;
                        }
                    }
                    else if (maxyear > obs_array[e].obs_date.year)
                    {
                        maxyear = obs_array[e].obs_date.year;
                        maxmonth = obs_array[e].obs_date.month;
                        maxday = obs_array[e].obs_date.day;
                        maxhour = obs_array[e].hour;
                        maxminute = obs_array[e].minute;
                    }
                }
                else if (maxtemp < obs_array[e].temperature)
                {
                    maxtemp = obs_array[e].temperature;
                    maxyear = obs_array[e].obs_date.year;
                    maxmonth = obs_array[e].obs_date.month;
                    maxday = obs_array[e].obs_date.day;
                    maxhour = obs_array[e].hour;
                    maxminute = obs_array[e].minute;
                }

                if (mintemp == obs_array[e].temperature)
                {
                    if (minyear == obs_array[e].obs_date.year)
                    {
                        if (minmonth == obs_array[e].obs_date.month)
                        {
                            if (minday == obs_array[e].obs_date.day)
                            {
                                if (minhour == obs_array[e].hour)
                                {
                                    if (minminute > obs_array[e].minute)
                                    {
                                        minminute = obs_array[e].minute;
                                    }
                                }
                                else if (minhour > obs_array[e].hour)
                                {
                                    minhour = obs_array[e].hour;
                                    minminute = obs_array[e].minute;
                                }
                            }
                            else if (minday > obs_array[e].obs_date.day)
                            {
                                minday = obs_array[e].obs_date.day;
                                minhour = obs_array[e].hour;
                                minminute = obs_array[e].minute;
                            }
                        }
                        else if (minmonth > obs_array[e].obs_date.month)
                        {
                            minmonth = obs_array[e].obs_date.month;
                            minday = obs_array[e].obs_date.day;
                            minhour = obs_array[e].hour;
                            minminute = obs_array[e].minute;
                        }
                    }
                    else if (minyear > obs_array[e].obs_date.year)
                    {
                        minyear = obs_array[e].obs_date.year;
                        minmonth = obs_array[e].obs_date.month;
                        minday = obs_array[e].obs_date.day;
                        minhour = obs_array[e].hour;
                        minminute = obs_array[e].minute;
                    }
                }
                else if (mintemp > obs_array[e].temperature || mintemp == 100)
                {
                    mintemp = obs_array[e].temperature;
                    minyear = obs_array[e].obs_date.year;
                    minmonth = obs_array[e].obs_date.month;
                    minday = obs_array[e].obs_date.day;
                    minhour = obs_array[e].hour;
                    minminute = obs_array[e].minute;
                }
            }
        }

        if (statnum != 251)
        {
            printf("Station %d: Minimum = %.2f degrees (%04d-%02d-%02d %02d:%02d), Maximum = %.2f degrees (%04d-%02d-%02d %02d:%02d)\n", statnum, mintemp, minyear, minmonth, minday, minhour, minminute, maxtemp, maxyear, maxmonth, maxday, maxhour, maxminute);
        }
        else
        {
            i = num_observations;
        }
        statnum = 251;
    }
}
/* print_daily_averages(num_observations, obs_array)
   Given an array of observation objects, compute and print the average
   temperature for each day which has at least one observation in the
   dataset.

   The output must contain only dates which actually appear in at least
   one observation object in the array. The dates must be in ascending
   chronological order (so an earlier date must be printed before a later
   one) and each date may only appear once.

   Each line of output will have the form "year month day average", for
   example "2022 11 20 10.6" (which would be interpreted to mean that
   the average temperature on Nov. 21, 2022 was 10.6 degrees).

   Your code may assume that all dates are from years between 2015 and 2022
   (inclusive) and that the month/day entries are all valid (that is, month
   will always be between 1 and 12 inclusive and day will always be between
   1 and 31 inclusive).

   This function must _not_ use any file I/O features whatsoever.

   Parameters: num_observations (integer), observation_array (array of Observation)
   Return value: None
   Side Effect: A printed representation of the average daily temperature is
                output to the user.
*/
void print_daily_averages(int num_observations, Observation obs_array[num_observations])
{

    int year = 2015;
    int month = 1;
    int day = 1;
    int i = 0;
    double temp = 0.0;
    double counter = 0.0;

    for (year = year; year <= 2022; year++)
    {
        for (month = month; month <= 12; month++)
        {
            for (day = day; day <= 31; day++)
            {
                while (i <= num_observations)
                {
                    if (year == obs_array[i].obs_date.year && month == obs_array[i].obs_date.month && day == obs_array[i].obs_date.day)
                    {
                        temp = temp + obs_array[i].temperature;
                        counter++;
                        i++;
                    }
                    else
                    {
                        i++;
                    }
                }
                if (counter != 0)
                {
                    printf("%d %d %d %.1f\n", year, month, day, temp/counter);
                }
                i = 0;
                temp = 0;
                counter = 0;
            }
            day = 0;
        }
        month = 0;
    }
}