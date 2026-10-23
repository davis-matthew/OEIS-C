/*
 * A141039  Years in which there are five Wednesdays in the month of February.
 * https://oeis.org/A141039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   feb1_is_wed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th year y ≥ 1753 such that
 *                y is leap (y%4==0 && (y%100!=0 || y%400==0))
 *                and February 1 of y is a Wednesday.
 *             Sunday‑based day of week is used: 0=Saturday, ..., 4=Wednesday.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/feb1_is_wed.h"

static inline bool is_leap(idx_t y) {
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

seq_t a(idx_t n) {
    /* static cache of previously generated terms */
    static seq_t *cache = NULL;
    static idx_t cache_count = 0;
    static size_t cache_cap = 0;
    static idx_t next_year = 1753;   /* first year to examine */

    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* Avoid compiler warnings about unused static variables */
        (void)cache_count;
    }

    /* Generate terms until we have at least n of them */
    while (cache_count < n) {
        if (is_leap(next_year) && feb1_is_wed(next_year)) {
            if (cache_count == cache_cap) {
                /* grow cache */
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) { /* memory exhaustion: abort */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_count++] = (seq_t)next_year;
        }
        next_year++; /* examine next year */
    }

    return cache[n - 1];
}
