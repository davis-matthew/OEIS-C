/*
 * A094213  a(n) = Sum_{k=0..n} binomial(9*n,9*k).
 * https://oeis.org/A094213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} binomial(9*n, 9*k) Recurrence (order 5): a(n)
 *            = 265*a(n-1) + 139823*a(n-2) - 6826204*a(n-3) - 6965249*a(n-4) +
 *            512*a(n-5) Initial values: a(0)=1, a(1)=2, a(2)=48622,
 *            a(3)=9373652, a(4)=9263421862
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, free, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with the first five terms if needed */
    if (cache_size == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed, return 0 */

        cache[0] = (seq_t)1ULL;
        cache[1] = (seq_t)2ULL;
        cache[2] = (seq_t)48622ULL;
        cache[3] = (seq_t)9373652ULL;
        cache[4] = (seq_t)9263421862ULL;
        cache_size = 5;
    }

    /* if we haven't built enough terms, extend the cache */
    if (n >= cache_size) {
        /* grow capacity if needed */
        if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            while (new_cap <= (size_t)n + 1) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute terms from cache_size to n inclusive */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* only need to compute for i >= 5 */
            seq_t term = 0;
            term += (seq_t)265ULL * cache[i-1];
            term += (seq_t)139823ULL * cache[i-2];
            term -= (seq_t)6826204ULL * cache[i-3];
            term -= (seq_t)6965249ULL * cache[i-4];
            term += (seq_t)512ULL * cache[i-5];
            cache[i] = term;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
