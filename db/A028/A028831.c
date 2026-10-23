/*
 * A028831  Expansion of g.f. (1+2*x+3*x^2)/(1-x-x^2-x^3-x^4).
 * https://oeis.org/A028831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1)+a(n‑2)+a(n‑3)+a(n‑4) for n≥4, a(0)=1, a(1)=3,
 *            a(2)=7, a(3)=11
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of valid entries in cache */
    static size_t cache_cap = 0; /* allocated capacity */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;               /* out_of_memory */
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 7;
        cache[3] = 11;
        cache_len = 4;
    }

    /* if the requested index is already available, return it */
    if ((size_t)n < cache_len)
        return cache[n];

    /* extend the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        cache_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* out_of_memory */
        cache = tmp;
    }

    /* compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4];
    }

    /* update the number of computed entries */
    cache_len = (size_t)n + 1;

    return cache[n];
}
