/*
 * A077241  Combined Diophantine Chebyshev sequences A054488 and A077413.
 * https://oeis.org/A077241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) for n>=4, a(0)=1, a(1)=2, a(2)=8,
 *            a(3)=13 (derived from the generating function
 *            (1+x)*(1+x+x^2)/(1-6*x^2+x^4)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;         /* array of computed terms   */
    static idx_t cache_size = 0;        /* number of terms allocated */
    static idx_t computed_up_to = 0;    /* last index computed       */

    /* Initialise cache with first four terms */
    if (cache_size == 0)
    {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* fallback, unlikely to happen */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)13;
        computed_up_to = 3;
    }

    /* If the requested term is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to accommodate n+1 terms */
    if (cache_size <= n)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* fallback */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i)
    {
        /* Recurrence: a(i) = 6*a(i-2) - a(i-4) */
        seq_t term = (seq_t)6 * cache[i - 2] - cache[i - 4];
        cache[i] = term;
    }
    computed_up_to = n;

    return cache[n];
}
