/*
 * A163271  Numerators of fractions in a 'zero-transform' approximation of
 *          sqrt(2) by means of a(n) = (a(n-1) + c)/(a(n-1) + 1) with c=2 and
 *          a(1)=0.
 * https://oeis.org/A163271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) (a(1)=0, a(2)=2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* below the offset – behaviour irrelevant */

    static seq_t *cache = NULL;          /* memoisation table */
    static idx_t cache_limit = 0;        /* highest index stored in cache */

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        /* Allocate space for indices 0,1,2 */
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)0;             /* unused slot */
        cache[1] = (seq_t)0;             /* a(1) */
        cache[2] = (seq_t)2;             /* a(2) */
        cache_limit = 2;
    }

    /* Return a cached value if available */
    if (n <= cache_limit)
        return cache[n];

    /* Grow the cache to accommodate the requested index */
    idx_t new_limit = n;
    seq_t *tmp = (seq_t*)realloc(cache,
                                 (size_t)(new_limit + 1) * sizeof(seq_t));
    if (!tmp) return (seq_t)0;           /* allocation failed */
    cache = tmp;

    /* Compute the missing terms using the recurrence */
    for (idx_t i = cache_limit + 1; i <= new_limit; ++i)
        cache[i] = cache[i - 1] * 2 + cache[i - 2];

    cache_limit = new_limit;
    return cache[n];
}
