/*
 * A141039  Years in which there are five Wednesdays in the month of February.
 * https://oeis.org/A141039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/*  Zeller's congruence for Gregorian calendar (month 14 = February). */
static inline bool feb1_is_wed(idx_t y) {
    idx_t Y = y - 1;            /* year for Feb (month 14) */
    idx_t K = Y % 100;          /* year of the century */
    idx_t J = Y / 100;          /* zero‑based century */
    /* h: 0 = Saturday, 1 = Sunday, ... 6 = Friday */
    idx_t h = (1 + 39 + K + K / 4 + J / 4 + 5 * J) % 7;
    return h == 4;              /* 4 corresponds to Wednesday */
}

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
        cache = mem_new(seq_t, cache_cap);
        /* Avoid compiler warnings about unused static variables */
        (void)cache_count;
    }

    /* Generate terms until we have at least n of them */
    while (cache_count < n) {
        if (is_leap(next_year) && feb1_is_wed(next_year)) {
            if (cache_count == cache_cap) {
                /* grow cache */
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
