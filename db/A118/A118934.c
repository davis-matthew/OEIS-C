/*
 * A118934  E.g.f.: exp(x + x^4/4).
 * https://oeis.org/A118934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (n-1)*(n-2)*(n-3)*a(n-4), a(0)=a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization cache: holds all values computed so far.
       cache_len is the number of valid entries in cache. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* First call: initialise the cache with the 4 initial values. */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
    }

    /* Grow the cache if the requested index is beyond what we have. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;

        /* Compute new values in increasing order, using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 4) {
                cache[i] = (seq_t)1;           /* initial values */
                continue;
            }
            seq_t mul = (seq_t)(i-1) * (seq_t)(i-2) * (seq_t)(i-3);
            cache[i] = cache[i-1] + mul * cache[i-4];
        }
        cache_len = new_len;
    }

    return cache[n];
}
