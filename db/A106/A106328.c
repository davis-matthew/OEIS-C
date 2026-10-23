/*
 * A106328  Numbers j such that 8*(j^2) + 9 = k^2 for some positive number k.
 * https://oeis.org/A106328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0, a(2) = 3 a(n) = 6*a(n-1) - a(n-2) (for n ≥ 3) This
 *            recurrence generates sequence A106328
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache values to handle calls in arbitrary order. */
    static seq_t *cache = NULL;   /* 1‑based array, cache[1] = a(1), etc. */
    static idx_t cache_size = 0;  /* highest index computed so far */

    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;

    /* Initialize the cache on first call. */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc((size_t)(cache_size + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failed */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
    }

    /* Grow the cache if necessary. */
    if (cache_size < n) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(new_size + 1) * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;  /* allocation failed */
        cache = new_cache;
        /* Compute missing terms using the recurrence. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)6 * cache[i-1] - cache[i-2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
