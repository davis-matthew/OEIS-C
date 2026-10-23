/*
 * A143995  Years in which there are five Thursdays in the month of February,
 *          in the Gregorian calendar.
 * https://oeis.org/A143995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Years in which February has five Thursdays in the Gregorian calendar.
   The n‑th such year is found by searching forward for a leap year whose
   February 1 falls on a Thursday.  Search starts from 1752, the earliest
   year that follows the Gregorian adoption in most countries. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms   */
    static size_t cache_cap = 0;   /* allocated capacity       */
    static idx_t search_year = 1752; /* first year to examine   */

    /* Grow the cache until it contains the desired term */
    while (cache_len < n) {
        for (;;) {
            /* Leap‑year test */
            if (((search_year % 4ULL == 0ULL) && (search_year % 100ULL != 0ULL))
                || (search_year % 400ULL == 0ULL)) {

                /* Day of week for February 1.
                   0 = Monday, 3 = Thursday. */
                idx_t y1   = search_year - 1ULL;
                idx_t days = y1 * 365ULL
                           + y1 / 4ULL
                           - y1 / 100ULL
                           + y1 / 400ULL
                           + 31ULL;
                if (days % 7ULL == 3ULL) {      /* Thursday */
                    /* Grow cache if necessary */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                        seq_t *tmp = (seq_t *)realloc(cache,
                                                      new_cap * sizeof(seq_t));
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = (seq_t)search_year;
                    /* Advance search start for next call */
                    ++search_year;
                    break; /* found the next term */
                }
            }
            ++search_year; /* try next year */
        }
    }

    return cache[n - 1];
}
