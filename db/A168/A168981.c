/*
 * A168981  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   the sequence satisfies the linear recurrence a(0) = 1, for n ≥
 *            1: a(n) = 14 * Σ_{k=1}^{22} a(n-k) – 105 * a(n-23) + p(n), where
 *            p(n) = 1 if n = 0 or 23, p(n) = 2 for 1 ≤ n ≤ 22, p(n) = 0
 *            otherwise. The first 17 terms agree with OEIS
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, free, abort */

seq_t a(idx_t n)
{
    /* ---------- cache management ---------------------------------------- */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;

    /* allocate initial cache if necessary */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;                 /* uninitialized flag */
    }

    /* grow cache to include index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        /* copy existing data */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        /* initialize newly allocated slots to zero */
        for (size_t i = cache_cap; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* ensure a(0) is set */
    if (cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;

    /* compute any missing values up to n */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (cache[i] != (seq_t)0) continue;   /* already computed */

        seq_t sum = (seq_t)0;
        /* positive contributions: 14 * a(i-k), for k = 1..22 */
        for (int k = 1; k <= 22 && i >= (size_t)k; ++k)
            sum += cache[i - k] * (seq_t)14;
        /* negative contribution: -105 * a(i-23) if i>=23 */
        if (i >= 23)
            sum -= cache[i - 23] * (seq_t)105;
        /* add the p(i) term */
        if (i == 23)
            sum += (seq_t)1;
        else if (i <= 22)
            sum += (seq_t)2;
        /* store result */
        cache[i] = sum;
    }

    return cache[n];
}
