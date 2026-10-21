/*
 * A077978  Expansion of 1/(1+x+2*x^2-x^3).
 * https://oeis.org/A077978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) satisfies a(0)=1, a(1)=-1, a(2)=-1 and a(n) + a(n-1) + 2
 *            a(n-2) - a(n-3) = 0 for n >= 3
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 47, i64 n <= 94, i128 n <= 190,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t used = 0;           /* number of terms currently stored */

    /* initialize the cache with the first three terms */
    if (!cache) {
        used = 3;
        cache = (seq_t *)malloc(used * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure; unlikely in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-1;
    }

    /* if the requested term is already known, return it immediately */
    if (n < used)
        return cache[n];

    /* grow the cache to accommodate the requested index */
    idx_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!cache) return (seq_t)0; /* allocation failure */

    /* compute missing terms up to n using the linear recurrence */
    for (idx_t i = used; i <= n; ++i) {
        /* a(i) = -a(i-1) - 2*a(i-2) + a(i-3) */
        cache[i] = -(cache[i-1]) - (cache[i-2] * (seq_t)2) + cache[i-3];
    }

    used = new_size;
    return cache[n];
}
