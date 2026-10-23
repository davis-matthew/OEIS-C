/*
 * A174770  y-values in the solution to x^2 - 29*y^2 = 1.
 * https://oeis.org/A174770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19602*a(n-1) - a(n-2), a(1)=0, a(2)=1820
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms.  Index 0 holds a(1). */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) {
        /* Offset is 1; behaviour for n<1 does not matter. */
        return (seq_t)0;
    }

    /* Allocate or grow the cache to hold at least n terms. */
    if (cache == NULL) {
        cache_cap = (size_t)n;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* malloc failure => 0 */
    } else if ((size_t)n > cache_cap) {
        cache_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* realloc failure => 0 */
        cache = tmp;
    }

    /* Compute terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        if (cache_len == 0) {
            cache[0] = (seq_t)0;                   /* a(1) */
        } else if (cache_len == 1) {
            cache[1] = (seq_t)1820;                /* a(2) */
        } else {
            cache[cache_len] =
                (19602 * cache[cache_len - 1]) - cache[cache_len - 2];
        }
        cache_len++;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
