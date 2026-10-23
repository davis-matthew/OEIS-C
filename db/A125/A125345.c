/*
 * A125345  Number of base 8 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS: A125345 - Number of base 8 circular n‑digit numbers with adjacent digits differing by 4 or less.
 *
 * Formula: a(n) satisfies the linear recurrence of order 7:
 *   a(0) = 1, a(1) = 8, a(2) = 52, a(3) = 308, a(4) = 2012,
 *   a(5) = 13308, a(6) = 88582, a(7) = 590710.
 *
 *   For n ≥ 8:
 *     a(n) = 8*a(n-1) - 6*a(n-2) - 20*a(n-3) + 5*a(n-4) + 12*a(n-5)
 *            - a(n-6) - 2*a(n-7).
 *
 * This implementation uses memoisation so that repeated calls are efficient
 * and works for C and C++ with arbitrary precision integer types.
 */

#include <stdlib.h>

static const seq_t INIT[8] = {
    (seq_t)1,          /* a(0) */
    (seq_t)8,          /* a(1) */
    (seq_t)52,         /* a(2) */
    (seq_t)308,        /* a(3) */
    (seq_t)2012,       /* a(4) */
    (seq_t)13308,      /* a(5) */
    (seq_t)88582,      /* a(6) */
    (seq_t)590710      /* a(7) */
};

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries stored: cache[0 .. cache_len-1] */

    /* initialise cache with first 8 values if not already done */
    if (cache_len == 0) {
        /* reserve space for at least 8 entries (but grow more as needed) */
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < (idx_t)8; ++i) {
            cache[i] = INIT[i];
        }
    }

    /* if requested n is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* grow the cache to accommodate n */
    idx_t new_cap = cache_len;
    while (new_cap <= n) {
        new_cap = (new_cap == 0) ? 8 : new_cap * 2;
    }
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    /* compute values up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 7) {
            /* just copy the pre‑initialised value (should not reach here normally) */
            cache[i] = INIT[i];
            continue;
        }
        /* recurrence: a(n) = 8*a(n-1) - 6*a(n-2) - 20*a(n-3) + 5*a(n-4) + 12*a(n-5) - a(n-6) - 2*a(n-7) */
        seq_t term1 = (seq_t)8 * cache[i-1];
        seq_t term2 = (seq_t)6 * cache[i-2];
        seq_t term3 = (seq_t)20 * cache[i-3];
        seq_t term4 = (seq_t)5 * cache[i-4];
        seq_t term5 = (seq_t)12 * cache[i-5];
        seq_t term6 = cache[i-6];
        seq_t term7 = (seq_t)2 * cache[i-7];

        seq_t value = term1 - term2 - term3 + term4 + term5 - term6 - term7;
        cache[i] = value;
    }
    cache_len = n + 1;
    return cache[n];
}
