/*
 * A168072  Expansion of (1-27x^2-108x^3)/((1-3x)^2*(1+3x+9x^2)).
 * https://oeis.org/A168072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 7, i32 n <= 16, i64 n <= 35, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A168072
   Expansion of (1-27x^2-108x^3)/((1-3x)^2*(1+3x+9x^2)).
   Recurrence (for n ≥ 4):
       a(n) = 3*a(n-1) + 27*a(n-3) - 81*a(n-4),
   with initial values:
       a(0)=1, a(1)=3, a(2)=-18, a(3)=-135. */
#ifndef A168072_H
#define A168072_H

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;          /* allocated array size */
    static idx_t last = 0;              /* highest index stored (>=3 after init) */
    static int init = 0;                /* flag: cache initialised */

    /* initialise cache on first call */
    if (!init) {
        /* start with space for 4 values */
        capacity = 4;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        /* initial terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)-18;
        cache[3] = (seq_t)-135;
        last = 3;
        init = 1;
    }
    /* grow cache if needed */
    if (n >= capacity) {
        idx_t new_cap = capacity ? capacity : 4;
        /* avoid overflow when doubling */
        while (new_cap <= n && new_cap <= (UINT64_MAX >> 1))
            new_cap <<= 1;
        /* if still insufficient, cap at max */
        if (new_cap <= n) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* compute missing values up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* recurrence valid for i >= 4 */
            seq_t term1 = (seq_t)3 * cache[i - 1];
            seq_t term2 = (seq_t)27 * cache[i - 3];
            seq_t term3 = (seq_t)81 * cache[i - 4];
            cache[i] = term1 + term2 - term3;
        }
        last = n;
    }

    return cache[n];
}

#endif /* A168072_H */
