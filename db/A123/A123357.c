/*
 * A123357  Kekulé numbers for certain benzenoids (see the Cyvin-Gutman book
 *          for details).
 * https://oeis.org/A123357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) for n >= 2 with a(0) = 1, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoized values */
    static idx_t  upto = 0;          /* maximum index currently stored */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        size_t init_size = 2;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* on allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        upto = 1;
    }

    /* if we already have the requested value, return it */
    if (n <= upto)
        return cache[(size_t)n];

    /* grow the cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > (size_t)upto + 1) {
        size_t new_size = (size_t)upto + 1;
        while (new_size < need)
            new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* on allocation failure */
        cache = tmp;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = upto + 1; i <= n; ++i) {
        cache[(size_t)i] =
            (seq_t)8 * cache[(size_t)(i-1)] - (seq_t)8 * cache[(size_t)(i-2)];
    }
    upto = n;

    return cache[(size_t)n];
}
