/*
 * A206066  Number of (n+1) X 3 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three clockwise edge increases.
 * https://oeis.org/A206066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 8*a(n-2) for n > 6 Initial values: a(1)=150,
 *            a(2)=606, a(3)=3330, a(4)=18534, a(5)=103194, a(6)=574182
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state to support arbitrary order of calls */
    static seq_t *cache      = NULL;   /* cache[1..max_cached] holds computed terms */
    static idx_t  cache_size = 0;      /* allocated size (indexes 0..cache_size-1) */
    static idx_t  max_cached = 0;      /* highest index with a known value */

    /* Handle request for n == 0 (undefined in OEIS, return 0) */
    if (n == 0) {
        return (seq_t)0;
    }

    /* First call: allocate base cache of size 7 (indexes 0..6) and set the first 6 terms */
    if (cache_size == 0) {
        cache_size = 7;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */

        cache[1] = (seq_t)150;
        cache[2] = (seq_t)606;
        cache[3] = (seq_t)3330;
        cache[4] = (seq_t)18534;
        cache[5] = (seq_t)103194;
        cache[6] = (seq_t)574182;
        max_cached = 6;
    }

    /* Ensure the cache has room for up to index n */
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size <= n + 1) {
            new_size = n + 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache    = tmp;
        cache_size = new_size;
        /* new elements are uninitialised but will be filled when computed */
    }

    /* If the value is already computed, return it immediately */
    if (n <= max_cached) {
        return cache[n];
    }

    /* Compute missing terms up to n */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        if (k <= 6) {
            /* Base values are already set; just update max_cached */
            max_cached = k;
        } else {
            seq_t val = (seq_t)7 * cache[k - 1] - (seq_t)8 * cache[k - 2];
            cache[k] = val;
            max_cached = k;
        }
    }

    return cache[n];
}
