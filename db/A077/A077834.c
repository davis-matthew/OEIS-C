/*
 * A077834  Expansion of 1/(1 - 2*x - 2*x^2 - 3*x^3).
 * https://oeis.org/A077834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=6; a(n)=2*a(n-1)+2*a(n-2)+3*a(n-3) for n≥3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cachelen = 0;

    /* initialise the cache with the first three terms */
    if (cachelen == 0) {
        cachelen = 3;
        cache = (seq_t *)calloc(cachelen, sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
    }

    /* if requested term already cached, return it directly */
    if (n < cachelen)
        return cache[n];

    /* grow the cache to hold up to term n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure fallback – return zero */
        return 0;
    }
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cachelen; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)2
                 + cache[i - 2] * (seq_t)2
                 + cache[i - 3] * (seq_t)3;
    }
    cachelen = n + 1;

    return cache[n];
}
