/*
 * A052533  Expansion of (1-x)/(1-x-3*x^2).
 * https://oeis.org/A052533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2), with a(0) = 1, a(1) = 0
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed values */
static idx_t  cache_size = 0; /* number of entries in cache */

seq_t a(idx_t n)
{
    /* initialize the cache with the first two terms */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failed */
        cache[0] = 1;
        cache[1] = 0;
    }

    /* if already computed, return immediately */
    if (n < cache_size)
        return cache[n];

    /* grow the cache to size n+1 */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);                 /* allocation failed */
    cache = tmp;

    /* compute new terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i - 1] + (seq_t)3 * cache[i - 2];
    }

    cache_size = new_size;
    return cache[n];
}
