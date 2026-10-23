/*
 * A020361  Numbers k such that the continued fraction for sqrt(k) has period
 *          22.
 * https://oeis.org/A020361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1399, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that the continued fraction of sqrt(k)
   has period 22.  The function builds the sequence by testing
   successive integers, memoizing the results so that any order of
   calls is supported. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/int_sqrt.h"

/* Test whether the period of √D is exactly 22. */
static bool period_is_22(idx_t D)
{
    idx_t a0 = int_sqrt(D);
    if (a0 * a0 == D) {               /* perfect square → period 0 */
        return false;
    }

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;
    int period = 0;

    do {
        /* m = d*a - m */
        __uint128_t tmp = (__uint128_t)d * a - m;
        m = (idx_t)tmp;

        /* d = (D - m*m) / d */
        __uint128_t m2 = (__uint128_t)m * m;
        __uint128_t rem = (__uint128_t)D - m2;
        d = (idx_t)(rem / d);

        a = (a0 + m) / d;
        period++;

        /* Safety guard – should never trigger for legitimate D. */
        if (period > 2 * D) {
            return false;
        }
    } while (a != 2 * a0);

    return period == 22;
}

/* Required OEIS function. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n==0, return 0 */
        return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */
    static idx_t cap = 0;          /* allocated capacity */
    static idx_t last_checked = 0; /* last tested candidate */

    if (n <= cache_size)
        return cache[n - 1];

    /* Ensure space for at least one more element. */
    if (cache_size + 1 > cap) {
        size_t new_cap = cap ? (cap << 1) : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cap = new_cap;
    }

    idx_t cand;
    for (cand = last_checked + 1; cache_size < n; ++cand) {
        if (cand <= 1)                /* 1 and 0 are perfect squares */
            continue;
        if (period_is_22(cand)) {
            cache[cache_size++] = (seq_t)cand;
            /* Ensure capacity for next potential element. */
            if (cache_size + 1 > cap) {
                size_t new_cap = cap ? (cap << 1) : 64;
                seq_t *tmp = (seq_t *)realloc(cache,
                                                new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cap = new_cap;
            }
        }
    }
    last_checked = cand - 1;   /* prepare for next call */

    return cache[n - 1];
}
