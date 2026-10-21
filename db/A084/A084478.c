/*
 * A084478  Number of tilings of a 5 X 3n rectangle with right trominoes.
 * https://oeis.org/A084478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 103*a(n-2) + 280*a(n-3) + 380*a(n-4) for n > 4
 *            a(0) = 1, a(1) = 0, a(2) = 72, a(3) = 384, a(4) = 8544
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoized values */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;    /* highest index computed so far */
    static size_t cache_size = 0;  /* allocated size of cache array */

    /* First call: allocate cache and initialise the first five terms */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)72;
        cache[3] = (seq_t)384;
        cache[4] = (seq_t)8544;
        cache_max = 4;
    }

    /* If the requested index is already in the cache, return it */
    if (n <= cache_max) {
        return cache[n];
    }

    /* Grow the cache if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failed */
        cache = tmp;
        cache_size = needed;
    }

    /* Compute values up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        cache[i] = (2 * a1) + (103 * a2) + (280 * a3) + (380 * a4);
    }
    cache_max = n;
    return cache[n];
}
