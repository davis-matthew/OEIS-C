/*
 * A198832  Values x for records of maxima of positive distance d between a
 *          cube of positive integer x and a square of integer y and such that
 *          d = x^3 - y^2.
 * https://oeis.org/A198832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A198832
   a(n) = the n-th positive integer x such that
          d(x) = x³ – floor( sqrt(x³) )² is a record maximum
          over all previously seen values of x.
   The sequence lists the x values where d(x) attains a new record
   (starting with a(1)=1).  The required function provides the n-th
   term of this sequence.
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> records;
static seq_t max_d = 0;
static idx_t last_x = 0;
#else
static seq_t *records = NULL;
static size_t records_cap = 0;
static seq_t max_d = 0;
static idx_t last_x = 0;
static size_t rec_count = 0;
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n >= 1 */

#ifdef __cplusplus
    /* Extend the cache until we have at least n entries */
    while (records.size() < (size_t)n)
    {
        for (idx_t x = last_x + 1; records.size() < (size_t)n; ++x)
        {
            seq_t xsq = (seq_t)x;
            seq_t cube = xsq * xsq * xsq;
            seq_t y = floor_sqrt(cube);
            seq_t d = cube - y * y;

            if (records.empty() || d > max_d)
            {
                records.push_back((seq_t)x);
                max_d = d;
            }
            last_x = x;
        }
    }
    return records[n - 1];
#else
    /* Ensure capacity for the array of records */
    while (rec_count < (size_t)n)
    {
        /* Compute next x values and add new records when necessary */
        for (idx_t x = last_x + 1; rec_count < (size_t)n; ++x)
        {
            seq_t xsq = (seq_t)x;
            seq_t cube = xsq * xsq * xsq;
            seq_t y = floor_sqrt(cube);
            seq_t d = cube - y * y;

            if (rec_count == 0 || d > max_d)
            {
                if (rec_count >= records_cap)
                {
                    size_t new_cap = records_cap == 0 ? 8 : records_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(records, new_cap * sizeof(seq_t));
                    if (!tmp)
                    {
                        /* Allocation failure; return 0 as fallback */
                        return 0;
                    }
                    records = tmp;
                    records_cap = new_cap;
                }
                records[rec_count++] = (seq_t)x;
                max_d = d;
            }
            last_x = x;
        }
    }
    return records[n - 1];
#endif
}
