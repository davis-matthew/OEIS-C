/*
 * A160300  Numerator of Hermite(n, 2/31).
 * https://oeis.org/A160300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n+2) = 4*a(n+1) - 1922*(n+1)*a(n), a(0) = 1, a(1) = 4
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;      /* array of already computed terms */
    static size_t  cache_sz  = 0;         /* allocated size of cache   */
    static idx_t   max_comp  = 1;         /* highest index computed    */

    /* Initialisation – fill first two terms */
    if (cache == NULL) {
        cache_sz = 2;
        cache    = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) return 0;          /* malloc failure – return 0 */
        cache[0] = 1;
        cache[1] = 4;
    }

    /* Grow cache to required length if necessary */
    if ((size_t)n >= cache_sz) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return cache[cache_sz-1];     /* leave cache unchanged on failure */
        cache    = tmp;
        cache_sz = new_sz;
    }

    /* Compute missing terms using recurrence */
    if ((idx_t)n > max_comp) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_t term1 = 4 * cache[i-1];
            seq_t term2 = 1922 * ((seq_t)(i-1)) * cache[i-2];
            cache[i] = term1 - term2;
        }
        max_comp = n;
    }

    return cache[(size_t)n];
}
