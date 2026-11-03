/*
 * A016309  Expansion of 1/((1-2*x)*(1-6*x)*(1-12*x)).
 * https://oeis.org/A016309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 72*a(n-2) + 2^n, with a(0)=1, a(1)=20
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 497 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A016309: Expansion of 1/((1-2*x)*(1-6*x)*(1-12*x)) */
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;          /* array holding computed terms   */
    static size_t cache_len = 0;        /* number of entries stored       */
    static seq_t pow2 = 0;              /* 2^(cache_len-1)               */

    /* initialise on first call */
    if (cache_len == 0) {
        cache = mem_new(seq_t, 2);
        cache[0] = 1;                    /* a(0) */
        cache[1] = 20;                   /* a(1) */
        cache_len = 2;
        pow2 = (seq_t)2;                 /* 2^(2-1) = 2 for next iteration */
        /* the array may be grown on future calls */
    }

    /* extend the cache if n is beyond what we have computed */
    if ((idx_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = mem_new(seq_t, new_len);

        /* copy existing terms to new cache */
        if (cache) {
            mem_copy(new_cache, cache, cache_len * sizeof(seq_t));
            mem_free(cache);
        }
        cache = new_cache;

        /* compute missing terms using the recurrence */
        for (idx_t k = cache_len; k <= n; ++k) {
            pow2 = pow2 * (seq_t)2;                      /* now 2^k          */
            cache[k] = cache[k-1] * (seq_t)18
                       - cache[k-2] * (seq_t)72 + pow2;    /* recurrence      */
        }
        cache_len = new_len;
    }

    return cache[n];
}
