/*
 * A072043  a(n) = a(n-1)*a(n-2)*a(n-3)*(1+1/(n-3)), a(1)=a(2)=a(3)=1.
 * https://oeis.org/A072043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)*a(n-2)*a(n-3)*(n-2)/(n-3) Initial values:
 *            a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cached_count = 0;

    /* initialise cache for the first three terms */
    if (cached_count == 0) {
        cache_cap = 4;                       /* initial capacity */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;                /* out of memory */
        cache[0] = 1;                       /* a(1) */
        cache[1] = 1;                       /* a(2) */
        cache[2] = 1;                       /* a(3) */
        cached_count = 3;
    }

    if (n <= cached_count) {
        return cache[n-1];
    }

    /* Grow the cache to fit the requested index */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;                  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Iteratively compute values from the last cached index up to n */
    for (idx_t k = cached_count + 1; k <= n; ++k) {
        /* a(k) = a(k-1)*a(k-2)*a(k-3) * (k-2)/(k-3) */
        seq_t prod = cache[k-2];            /* a(k-1) */
        prod *= cache[k-3];                 /* * a(k-2) */
        prod *= cache[k-4];                 /* * a(k-3) */
        prod *= (seq_t)(k-2);               /* * (k-2) */
        prod /= (seq_t)(k-3);               /* / (k-3) */
        cache[k-1] = prod;                  /* store a(k) */
    }
    cached_count = n;
    return cache[n-1];
}
