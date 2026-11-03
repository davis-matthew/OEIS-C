/*
 * A176478  Years in which there are five Saturdays in the month of February.
 * https://oeis.org/A176478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176478: Years in which there are five Saturdays in February.
   The sequence lists leap years for which February 1 falls on a Saturday.
   Because the Gregorian calendar repeats the pattern every 28 or 40 years,
   a simple search over successive leap years is sufficient and fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Zeller's Congruence for the Gregorian calendar.
   For years after 1582 it returns 0 = Saturday, 1 = Sunday, …, 6 = Friday.
   We use month = 14 and year-1 for February. */
static idx_t dow_feb1(idx_t y)
{
    /* Month 14 represents February of the preceding year. */
    const idx_t m = 14;
    const idx_t Y = y - 1;
    const idx_t q = 1;                /* Day of month */
    const idx_t K = Y % 100;
    const idx_t J = Y / 100;
    const idx_t h = (q + (13 * (m + 1)) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
    return h;                         /* 0 = Saturday, … */
}

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
        cache = mem_new(seq_t, cache_cap);
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
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)year;
            last_year = year;
        }
        ++year;
    }

    return cache[n - 1];
}
