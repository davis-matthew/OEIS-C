/*
 * A137200  Number of ways to tile an n X 1 strip with 1 X 1 squares and 2 X 1
 *          dominoes with the restriction that no three consecutive tiles are
 *          of the same type.
 * https://oeis.org/A137200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) for n > 4 a(0) = 1, a(1) = 1, a(2) = 2,
 *            a(3) = 2, a(4) = 4
 * Range:     u8 n <= 17, u16 n <= 34, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache that holds all values that have been computed so far. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* allocated length of cache */
    static idx_t last_used = 0;   /* number of valid entries in cache
                                     (indices 0 .. last_used-1 are set) */

    /* Initialise the cache on the first call with the first five terms. */
    if (cache_len == 0) {
        cache_len = 5;
        cache = (seq_t*)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) abort();   /* out of memory – terminate */

        cache[0] = 1; /* a(0) */
        cache[1] = 1; /* a(1) */
        cache[2] = 2; /* a(2) */
        cache[3] = 2; /* a(3) */
        cache[4] = 4; /* a(4) */

        last_used = 5; /* indices 0 .. 4 are valid */
    }

    /* If the value is already computed, just return it. */
    if (n < last_used)
        return cache[n];

    /* Ensure the allocated array is large enough to hold up to n. */
    if ((idx_t)n + 1 > cache_len) {
        idx_t new_len = (idx_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = last_used; i <= n; ++i) {
        /* Recurrence: a(i) = a(i-1) + a(i-4) for i >= 5. */
        cache[i] = cache[i - 1] + cache[i - 4];
    }

    last_used = n + 1;
    return cache[n];
}
