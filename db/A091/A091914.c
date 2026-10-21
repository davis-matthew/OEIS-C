/*
 * A091914  a(n) = 2*a(n-1) + 12*a(n-2).
 * https://oeis.org/A091914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 12*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoizes computed terms to handle calls in any order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initialize memoization table on first call. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 1;   /* we will store a(0) and a(1) as the first two values */

    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Ensure the cache has at least two slots. */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        max_computed = 1;
    }

    /* Grow the cache if necessary. */
    if (cache_size <= n) {
        idx_t new_size = cache_size > 0 ? cache_size : 2;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)2 + cache[i - 2] * (seq_t)12;
    }
    max_computed = n;

    return cache[n];
}
