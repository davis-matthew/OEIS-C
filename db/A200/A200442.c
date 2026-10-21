/*
 * A200442  Expansion of 1/(1-31*x+x^2).
 * https://oeis.org/A200442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=31*a(n-1)-a(n-2) for n≥2; a(0)=1, a(1)=31
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = (idx_t)-1;

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)31;
        cache_max = 1;
    }

    /* Return directly if we already know this value */
    if (n <= cache_max) {
        return cache[n];
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms sequentially */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)31 * cache[i - 1] - cache[i - 2];
    }
    cache_max = n;
    return cache[n];
}
