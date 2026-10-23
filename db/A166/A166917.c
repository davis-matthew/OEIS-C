/*
 * A166917  a(n) = 20*a(n-1) - 64*a(n-2) for n > 1; a(0) = 85, a(1) = 1364.
 * https://oeis.org/A166917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 64*a(n-2), a(0) = 85, a(1) = 1364
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation – the array is indexed by n */
    static seq_t *cache       = NULL;
    static size_t cap         = 0;
    static idx_t  upto        = (idx_t)-1; /* sentinel for “not initialised” */

    /* initialise cache with the two seed values */
    if (upto == (idx_t)-1) {
        upto = 1;                 /* we have a(0) and a(1) stored */
        cap  = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = 85;            /* a(0) */
        cache[1] = 1364;          /* a(1) */
    }

    /* already computed? */
    if (n <= upto)
        return cache[n];

    /* grow the cache if required */
    if (n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n)
            newcap *= 2;
        seq_t *newcache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache = newcache;
        cap   = newcap;
    }

    /* compute missing terms iteratively */
    for (idx_t i = upto + 1; i <= n; ++i)
        cache[i] = (seq_t)(20 * cache[i-1] - 64 * cache[i-2]);

    upto = n;
    return cache[n];
}
