/*
 * A016262  Expansion of 1/((1-x)(1-9x)(1-11x)).
 * https://oeis.org/A016262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1)-119*a(n-2)+99*a(n-3) * Initial values: a(0)=1,
 *            a(1)=21, a(2)=322 * (This recurrence is equivalent to the
 *            expansion of 1/((1-x)(1-9x)(1-11x)).)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cached values enable fast random access */
    static seq_t *cache = NULL;
    static idx_t  cached_until = 2;  /* last computed index */

    /* Handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)322;

    /* Lazy allocation of cache */
    if (cache == NULL) {
        /* Allocate space for indices 0..2 first */
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)322;
        cached_until = 2;
    }

    /* Existing cache covers the requested index? */
    if (n <= cached_until) {
        return cache[(size_t)n];
    }

    /* Grow cache to accommodate up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;          /* realloc failure */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cached_until + 1; i <= n; ++i) {
        size_t ii = (size_t)i;           /* array index */
        seq_t t1 = (seq_t)99 * cache[ii - 3];
        t1 += (seq_t)21 * cache[ii - 1];
        cache[ii] = t1 - (seq_t)119 * cache[ii - 2];
    }
    cached_until = n;

    return cache[(size_t)n];
}
