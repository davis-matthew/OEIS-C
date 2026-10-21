/*
 * A096053  a(n) = (3*9^n - 1)/2.
 * https://oeis.org/A096053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 4, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0..cache_len-1] */
    static idx_t cache_len = 0;   /* number of elements stored */

    /* Return cached value if available */
    if (n < cache_len)
        return cache[n];

    /* Determine new length needed */
    size_t new_len = (size_t)n + 1;

    /* Allocate or grow the cache */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp && new_len > 0) {
        /* Allocation failed – compute without caching */
        seq_t val = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            val = (seq_t)(9 * val + 4);
        return val;
    }
    cache = tmp;

    /* Initialise the first term if this is the first allocation */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Compute missing terms up to n (inclusive) */
    for (idx_t i = cache_len; i <= n; ++i)
        cache[i] = (seq_t)(9 * cache[i - 1] + 4);

    cache_len = new_len;
    return cache[n];
}
