/*
 * A160335  Numerator of Hermite(n, 24/31).
 * https://oeis.org/A160335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 48, a(n) = 48*a(n-1) - 2*(n-1)*961*a(n-2) for n
 *            ≥ 2 (Numerator of Hermite(n, 24/31))
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  a(idx_t n) can be called in any order; results are memoised. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated size */
    static idx_t    last  = 0;   /* largest computed index */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* ensure at least two slots so that a(0) and a(1) exist */
        cap   = (n + 2 > 2 ? 2 : n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);

        cache[0] = (seq_t)1;          /* a(0) */
        if (cap > 1)
            cache[1] = (seq_t)48;     /* a(1) */
        last = (cap > 1) ? 1 : 0;
    }
    /* grow the cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 2;
        while (newcap <= (size_t)n)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t term1 = (seq_t)48 * cache[i - 1];
            seq_t coeff = (seq_t)2 * (seq_t)(i - 1) * (seq_t)961;
            seq_t term2 = coeff * cache[i - 2];
            cache[i] = term1 - term2;
        }
        last = n;
    }

    return cache[n];
}
