/*
 * A020978  Expansion of 1/((1-8*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A020978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 276*a(n-2) + 864*a(n-3) with the initial
 *            values a(0)=1, a(1)=29, a(2)=565
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to store previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of elements currently stored */

    /* First call: allocate cache for the initial three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) { /* allocation failed */
            return 0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)29;
        cache[2] = (seq_t)565;
    }

    /* If the requested index is already cached, return it immediately */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow the cache enough to hold up to index n */
    idx_t needed = n + 1;
    if (needed > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) { /* allocation failed */
            return 0;
        }
        cache = new_cache;
        /* Remember the old length before it gets overwritten */
        idx_t old_len = cache_len;
        cache_len = needed;

        /* Compute terms from the old length up to n */
        for (idx_t i = old_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
                continue;
            }
            if (i == 1) {
                cache[i] = (seq_t)29;
                continue;
            }
            if (i == 2) {
                cache[i] = (seq_t)565;
                continue;
            }
            /* a(i) = 29*a(i-1) - 276*a(i-2) + 864*a(i-3) */
            seq_t term1 = (seq_t)29 * cache[i-1];
            seq_t term2 = (seq_t)276 * cache[i-2];
            seq_t term3 = (seq_t)864 * cache[i-3];
            cache[i] = term1 + term3 - term2;  /* addition is safe, subtraction will not underflow */
        }
    } else {
        /* If no growth was needed, simply compute from the current length */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
                continue;
            }
            if (i == 1) {
                cache[i] = (seq_t)29;
                continue;
            }
            if (i == 2) {
                cache[i] = (seq_t)565;
                continue;
            }
            seq_t term1 = (seq_t)29 * cache[i-1];
            seq_t term2 = (seq_t)276 * cache[i-2];
            seq_t term3 = (seq_t)864 * cache[i-3];
            cache[i] = term1 + term3 - term2;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
