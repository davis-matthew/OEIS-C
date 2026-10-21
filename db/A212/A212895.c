/*
 * A212895  Number of (w,x,y,z) with all terms in {0,...,n} and (least
 *          gapsize)=2.
 * https://oeis.org/A212895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)-6*a(n-2)+4*a(n-3)-a(n-4) for n>=9; a(0)=0,
 *            a(1)=0, a(2)=2, a(3)=14, a(4)=58, a(5)=168, a(6)=376, a(7)=716,
 *            a(8)=1224
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 896, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t cached_to = 0;

    /* initialise base values on first call */
    if (cache == NULL) {
        cache_size = 9;                     /* indices 0 .. 8 (inclusive) */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)                 /* allocation failure */
            return (seq_t)0;               /* graceful fallback */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)58;
        cache[5] = (seq_t)168;
        cache[6] = (seq_t)376;
        cache[7] = (seq_t)716;
        cache[8] = (seq_t)1224;
        cached_to = 8;
    }

    /* return cached value if already computed */
    if (n <= cached_to)
        return cache[n];

    /* ensure enough space in cache */
    size_t need = (size_t)n + 1;
    if (need > cache_size) {
        cache_size = need;
        seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;                /* graceful fallback */
        cache = tmp;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cached_to + 1; i <= n; ++i) {
        seq_t term = 0;

        /* a(i) = 4*a(i-1)+4*a(i-3)-6*a(i-2)-a(i-4)  for i>=9 */
        /* Perform operations in an order that avoids temporary underflow */
        term = cache[i - 1] * (seq_t)4U;
        term += cache[i - 3] * (seq_t)4U;
        term -= cache[i - 2] * (seq_t)6U;
        term -= cache[i - 4];

        cache[i] = term;
    }

    cached_to = n;
    return cache[n];
}
