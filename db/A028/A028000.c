/*
 * A028000  Expansion of 1/((1-2*x)*(1-6*x)*(1-9*x)*(1-11*x)).
 * https://oeis.org/A028000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 271*a(n-2) + 1032*a(n-3) - 1188*a(n-4);
 *            Initial values: a(0)=1, a(1)=28, a(2)=513, a(3)=7808
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static const seq_t init[4] = { (seq_t)1, (seq_t)28, (seq_t)513, (seq_t)7808 };

    /* Ensure cache is allocated and initialized */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory */
        /* store initial terms */
        for (size_t i = 0; i < 4 && i < cache_cap; ++i)
            cache[i] = init[i];
        cache_len = (cache_cap < 4) ? cache_cap : 4;
    }

    /* If we have already computed the requested term, return it */
    if (n < cache_len)
        return cache[n];

    /* Resize cache if required */
    if ((idx_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while ((idx_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i < 4) {
            cache[i] = init[i];
        } else {
            seq_t t = (seq_t)28   * cache[i-1];
            t -= (seq_t)271 * cache[i-2];
            t += (seq_t)1032 * cache[i-3];
            t -= (seq_t)1188 * cache[i-4];
            cache[i] = t;
        }
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
