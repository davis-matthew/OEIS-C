/*
 * A169432  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 33*Σ_{k=1}^{31}a(n-k) - 561*a(n-32) + r(n) where r(0)=1,
 *            r(1…31)=2, r(32)=1, r(n>32)=0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   coeff_N
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/coeff_N.h"

/* Static cache for memoisation */
static seq_t *cache      = NULL;
static size_t  cache_size = 0;
static seq_t   sum_last31 = (seq_t)0;

/* resize the cache so that it can hold indices up to new_size-1 */
static void ensure_capacity(idx_t new_size)
{
#ifdef __cplusplus
    if ((size_t)new_size <= cache_size) return;
    seq_t *new_cache = new seq_t[new_size];
    for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
    delete[] cache;
    cache   = new_cache;
    cache_size = new_size;
#else
    if ((size_t)new_size <= cache_size) return;
    seq_t *new_cache = realloc(cache, new_size * sizeof(seq_t));
    cache   = new_cache;
    cache_size = new_size;
#endif
}

seq_t a(idx_t n)
{
    /* if we already have the value, just return it */
    if (cache_size > (size_t)n) return cache[n];

    /* remember the size before we grow the cache */
    size_t old_size = cache_size;

    /* grow the cache to hold indices up to n */
    ensure_capacity(n + 1);

    /* compute all missing terms, in order */
    for (idx_t i = old_size; i <= n; ++i)
    {
        seq_t coef   = coeff_N(i);
        seq_t prev32 = (i >= 32) ? cache[i - 32] : (seq_t)0;

        /* a(i) = 33*sum_last31 - 561*prev32 + coef */
        seq_t val = (seq_t)33 * sum_last31;
        val = val - (seq_t)561 * prev32 + coef;
        cache[i] = val;

        /* update the sliding sum for the next index */
        sum_last31 = sum_last31 + val;
        if (i >= 31) sum_last31 = sum_last31 - cache[i - 31];
    }

    return cache[n];
}
