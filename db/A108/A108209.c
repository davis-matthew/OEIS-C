/*
 * A108209  a(n) = 5*(n-1)*a(n-1) -2*(n-1)*a(n-2) with a(0)=0, a(1)=2.
 * https://oeis.org/A108209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n-1)*a(n-1) - 2*(n-1)*a(n-2), a(0) = 0, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute A108209. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* If we already have a(n), return it. */
    if (n < cache_len)
        return cache[n];

    /* Need to compute up to n. Allocate larger cache. */
    size_t newlen = (size_t)n + 1;
    seq_t *newcache = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    if (!newcache)           /* allocation failure – return 0 (unlikely in tests) */
        return (seq_t)0;
    cache = newcache;

    /* Initialize base cases if necessary. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        if (newlen > 1)
            cache[1] = (seq_t)2;
    }
    /* If we had only one base term, ensure we have the second as well. */
    if (newlen > 1 && cache_len < 2)
        cache[1] = (seq_t)2;

    /* Compute missing terms from the first missing index up to n. */
    for (size_t i = (cache_len < 2 ? 2 : cache_len); i <= n; ++i) {
        seq_t im1 = (seq_t)(i - 1);
        seq_t term = 5 * im1 * cache[i - 1] - 2 * im1 * cache[i - 2];
        cache[i] = term;
    }

    cache_len = newlen;
    return cache[n];
}
