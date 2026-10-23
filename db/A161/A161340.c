/*
 * A161340  Number of "ON" cells at n-th stage of three-dimensional version of
 *          the cellular automaton A160414 using cubes.
 * https://oeis.org/A161340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(1) = 1 a(2^k) = (2·2^k – 1)^3 a(n) = a(n−1) + 56·K(n)
 *            if n is not a power of two where K(n) is computed from the
 *            binary decomposition of n (the same definition as used in the
 *            recurrence in the OEIS entry). The code uses an expanding cache
 *            so that arbitrary order of calls is supported e...
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow7_of_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow7_of_exp.h"

/* Helper: floor of binary logarithm for idx_t (uint64_t) */
static inline idx_t floor_log2_uint64(idx_t n)
{
    /* n > 0 when called */
    idx_t r = 0;
    while (n >>= 1)
        ++r;
    return r;
}

/*  The sequence function */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;      /* highest index stored (inclusive) */

    /* Ensure the cache is initialized for at least index 0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();       /* allocation failed */
        cache[0] = (seq_t)0;
        cache_max = 0;
    }

    /* Extend the cache if needed */
    if (n > cache_max) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();    /* allocation failed */
        cache = new_cache;

        /* compute values for new indices */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if ((i & (i - 1)) == 0) {           /* power of two */
                seq_t val = (seq_t)2 * (seq_t)i - (seq_t)1;    /* 2*i-1 */
                cache[i] = val * val * val;            /* (2*i-1)^3 */
            } else {
                /* compute incremental factor K(i) */
                idx_t logp = floor_log2_uint64(i);
                idx_t p = (idx_t)1 << logp;      /* highest power of two <= i */
                idx_t d = i - p;

                seq_t factor = 1;
                idx_t block = p;
                idx_t rest = d;

                while (1) {
                    if (block == 2) {               /* base block */
                        break;
                    }
                    if (rest == 0) {
                        idx_t exp = floor_log2_uint64(block) - 1;
                        factor *= pow7_of_exp(exp);
                        break;
                    }
                    idx_t half = block >> 1;
                    if (rest < half) {
                        block = half;
                    } else {
                        factor *= (seq_t)7;
                        rest -= half;
                        block = half;
                    }
                }

                seq_t diff = (seq_t)56 * factor;
                cache[i] = cache[i - 1] + diff;
            }
        }
        cache_max = n;
    }

    return cache[n];
}
