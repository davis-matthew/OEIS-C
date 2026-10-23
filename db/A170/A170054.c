/*
 * A170054  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170054:
   a(0) = 1
   For n ≤ 35: a(n) = 2 + 43 * Σ_{k=0}^{n-1} a(k)
   For n ≥ 36: a(n) = 22 * a(n-36) – Σ_{k=1}^{35} a(n-k)   */
   
#include <stdlib.h>              /* for abort() */
#include "../utils/seq_alloc.h"   /* seq_alloc returns a malloc‑like buffer */

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache   = NULL;   /* cached values  */
    static idx_t  cache_len = 0;    /* highest index computed + 1 */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* Grow the cache if required. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 32;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;

        seq_t *new_cache = seq_alloc(new_cap);
        if (!new_cache)
            abort();

        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        if (cache)
            /* seq_free(cache);  // not necessary – cache lives for the life of the program */
            ;

        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;

        if (i == 0) {
            /* a(0) = 1 */
            val = (seq_t)1U;
        }
        else if (i <= 35) {
            /* a(i) = 2 + 43 * Σ_{k=0}^{i-1} a(k) */
            seq_t sum = (seq_t)0U;
            for (idx_t j = 0; j < i; ++j)
                sum += cache[j];
            val = ((seq_t)2U) + ((seq_t)43U) * sum;
        }
        else {
            /* a(i) = 22 * a(i-36) – Σ_{k=1}^{35} a(i-k) */
            seq_t sum = (seq_t)0U;
            for (idx_t k = 1; k <= 35; ++k)
                sum += cache[i - k];
            val = ((seq_t)22U) * cache[i - 36] - sum;
        }

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
