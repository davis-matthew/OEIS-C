/*
 * A162241  Years in which January first falls on a Thursday (since 2001).
 * https://oeis.org/A162241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   leap_year
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = the nth year after 2001 in which January 1 falls on a Thursday
 * (search based; compute years one by one).
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/leap_year.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* state of the day‑of‑week generation */
    static idx_t last_year = 2001;   /* year of the last processed Jan 1 */
    static int   last_offset = 0;    /* 0 = Monday, 3 = Thursday */

    /* ensure cache is allocated */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n terms (1‑based) */
    while (cache_len < n) {
        /* advance to next year */
        last_year++;

        /* For the shift from the previous year to this one, use the
           leap‑year status of the previous year. */
        if (leap_year(last_year - 1))
            last_offset = (last_offset + 2) % 7;
        else
            last_offset = (last_offset + 1) % 7;

        /* Thursday corresponds to offset 3 (Monday = 0) */
        if (last_offset == 3) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_year;
        }
    }

    return cache[n - 1];
}
