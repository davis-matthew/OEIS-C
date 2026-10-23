/*
 * A180168  a(n) = 2*a(n-1) + 5*a(n-2), a(0) = 1, a(1) = 3.
 * https://oeis.org/A180168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2), a(0)=1, a(1)=3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of valid entries */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Initialize cache with the first two values */
    if (cache == NULL) {
        cache_cap  = 2;
        cache_size = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);  /* fatal: allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    /* Grow the cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < ((size_t)n + 1))
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) exit(EXIT_FAILURE);  /* fatal: allocation failed */
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    if ((size_t)n + 1 > cache_size) {
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            cache[i] = (seq_t)( (seq_t)(2) * cache[i - 1] + (seq_t)(5) * cache[i - 2] );
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
