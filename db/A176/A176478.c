/*
 * A176478  Years in which there are five Saturdays in the month of February.
 * https://oeis.org/A176478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   dow_feb1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176478: Years in which there are five Saturdays in February.
   The sequence lists leap years for which February 1 falls on a Saturday.
   Because the Gregorian calendar repeats the pattern every 28 or 40 years,
   a simple search over successive leap years is sufficient and fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/dow_feb1.h"

/* Utility: test whether y is a leap year in the Gregorian calendar. */
static bool is_leap(idx_t y)
{
    return ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0);
}

/* The sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t last_year = 0;        /* year of the last stored element */

    if (!cache) {                      /* initialise cache */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        last_year = 0;
    }

    /* Return cached result if it exists. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Start searching from the larger of the next year or the first year in the series. */
    idx_t year = (last_year < 1772) ? 1772 : last_year + 1;

    /* Search forward until we have computed n terms. */
    while (cache_len < n) {
        if (is_leap(year) && dow_feb1(year) == 0) { /* five Saturdays in Feb */
            if (cache_len == cache_cap) {          /* grow cache if needed */
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)year;
            last_year = year;
        }
        ++year;
    }

    return cache[n - 1];
}
