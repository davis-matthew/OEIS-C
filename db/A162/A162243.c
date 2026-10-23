/*
 * A162243  Years in which January first falls on a Saturday (since 2001).
 * https://oeis.org/A162243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth year >=2001 in which January 1 falls on a
 *            Saturday
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static bool is_leap_year(uint64_t y) {
    return (y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0));
}

seq_t a(idx_t n) {
    /* cache state */
    static seq_t *cache = NULL;     /* array of found years */
    static idx_t cache_size = 0;    /* number of years stored */
    static idx_t cache_cap = 0;     /* capacity of array */

    /* bookkeeping for incremental year generation */
    static uint64_t last_year = 2001;   /* year of the day stored in cur_day */
    static int cur_day = 0;             /* Jan 1 day of week for last_year:
                                          0=Monday, 1=Tuesday, …, 5=Saturday, 6=Sunday */

    /* extend cache until we have at least n terms */
    while (cache_size < n) {
        if (cur_day == 5) { /* Saturday */
            if (cache_size == cache_cap) {
                idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* reallocation must succeed; if it fails, the program will abort */
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)last_year;
        }

        /* advance to next year */
        last_year++;
        cur_day = (cur_day + (is_leap_year(last_year - 1) ? 2 : 1)) % 7;
    }

    return cache[n - 1];
}
