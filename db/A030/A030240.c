/*
 * A030240  Scaled Chebyshev U-polynomials evaluated at sqrt(7)/2.
 * https://oeis.org/A030240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=7; for n>=2, a(n)=7*a(n-1)-7*a(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed values */
static idx_t  cache_len = 0;      /* number of slots allocated in cache */

/* Compute the nth term of the sequence A030240 */
seq_t a(idx_t n)
{
    /* Return the known base values immediately */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    /* If the cache already covers n, just return it */
    if (cache_len > n) return cache[n];

    /* Expand the cache to at least n+1 entries */
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (new_cache == NULL) abort();          /* allocation failed */
    cache = new_cache;

    /* Initialize the first two entries if the cache was empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        if (new_len > 1) cache[1] = (seq_t)7;
    }

    /* Compute values from cache_len up to n */
    idx_t start = cache_len;
    if (start < 2) start = 2;                 /* we already have 0 and 1 */
    seq_t seven = 7;                          /* helper for multiplication */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = seven * cache[i - 1] - seven * cache[i - 2];
    }
    cache_len = new_len;                      /* remember the new size */

    return cache[n];
}
