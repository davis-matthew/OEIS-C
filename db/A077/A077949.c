/*
 * A077949  Expansion of 1/(1-x-2*x^3).
 * https://oeis.org/A077949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-3) (n ≥ 3), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 43, u64 n <= 85, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* INITIALISE cache on first call */
    if (cache_size == 0) {
        cache_size = 3;                      /* a(0), a(1), a(2) */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                /* unlikely: allocation failed */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
    }

    /* GROW cache if n is beyond the highest computed index */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* unlikely: allocation failed */
        cache = tmp;

        /* Compute missing terms */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i >= 3) {
                seq_t v = cache[i - 1];
                v += cache[i - 3];
                v += cache[i - 3];            /* v = a(i-1) + 2*a(i-3) */
                cache[i] = v;
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
