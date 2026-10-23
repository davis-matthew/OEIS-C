/*
 * A162239  Years in which January first falls on a Tuesday (since 2001).
 * https://oeis.org/A162239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Compute a(n) as the n-th year after 2001 in which January 1
 *            falls on a Tuesday (search algorithm)
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: determine whether a Gregorian year is a leap year. */
static bool is_leap_year(idx_t y)
{
    return ((y % 4ULL) == 0ULL) &&
           (((y % 100ULL) != 0ULL) || ((y % 400ULL) == 0ULL));
}

/* The sequence function. */
seq_t a(idx_t n)
{
    static idx_t cache_len = 0;      /* number of years stored */
    static idx_t cache_cap = 0;      /* allocated capacity */
    static seq_t *cache = NULL;      /* array of years */
    static idx_t next_year = 2001;   /* last processed year (start point) */
    static int next_day = 0;         /* day of week for Jan 1 of next_year:
                                       0=Mon, 1=Tue, …, 6=Sun */

    /* Grow cache if necessary */
    if (cache_len < n)
    {
        while (cache_cap < n)
        {
            idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return 0; /* allocation failure – undefined behavior */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Generate additional years until we have at least n terms */
        while (cache_len < n)
        {
            bool leap = is_leap_year(next_year);
            next_day = (next_day + (leap ? 2 : 1)) % 7;
            next_year++;

            if (next_day == 1)              /* 1 == Tuesday */
            {
                cache[cache_len++] = (seq_t)next_year;
            }
        }
    }

    return cache[n - 1];
}
