/*
 * A023955  Expansion of g.f. 1/((1-x)*(1-6*x)*(1-8*x)*(1-10*x)).
 * https://oeis.org/A023955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=25, a(2)=413, a(3)=5693 a(n) = 25*a(n-1) -
 *            212*a(n-2) + 668*a(n-3) - 480*a(n-4) for n ≥ 4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache the computed values for any ordering of calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* number of cached terms (size of cache) */

    if (n < cache_len)
        return cache[n];

    /* Allocate or grow the cache to hold terms up to n */
    idx_t new_len = n + 1;
    if (cache == NULL)
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    if (cache == NULL)  /* failure */
        return 0;

    /* Initialise initial four terms if required */
    if (cache_len < 4) {
        if (new_len > 0) cache[0] = 1;
        if (new_len > 1) cache[1] = 25;
        if (new_len > 2) cache[2] = 413;
        if (new_len > 3) cache[3] = 5693;
    }

    /* Compute missing terms using the recurrence */
    idx_t start = cache_len < 4 ? 4 : cache_len;
    for (idx_t k = start; k <= n; ++k) {
        seq_t term = 25 * cache[k-1];
        term = term - (212 * cache[k-2]);
        term = term + (668 * cache[k-3]);
        term = term - (480 * cache[k-4]);
        cache[k] = term;
    }

    cache_len = new_len;   /* update cached size */
    return cache[n];
}
