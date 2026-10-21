/*
 * A077864  Expansion of (1-x)^(-1)/(1-x-2*x^2-x^3).
 * https://oeis.org/A077864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=5, a(3)=11,
 *            a(n)=2*a(n-1)+a(n-2)-a(n-3)-a(n-4) for n>3 (OEIS A077864)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise the first four terms once */
    if (cache_len == 0) {
        cache = (seq_t *)realloc(cache, 4 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 5;
        cache[3] = 11;
        cache_len = 4;
    }

    /* Return if already computed */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to accommodate n */
    size_t new_len = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* Compute any new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 4)   /* safety: base values already set */
            continue;
        cache[i] = 2 * cache[i-1] + cache[i-2] - cache[i-3] - cache[i-4];
    }
    cache_len = new_len;
    return cache[n];
}
