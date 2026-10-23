/*
 * A160309  Numerator of Hermite(n, 11/31).
 * https://oeis.org/A160309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n+2) = 22*a(n+1) - 1922*(n+1)*a(n), a(0)=1, a(1)=22
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;          /* cache[0] = a(0), cache[1] = a(1), ... */
    static size_t cache_size = 0;        /* number of elements allocated */
    static idx_t computed_max;           /* highest index computed so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed – return 0 */
        cache[0] = 1;
        cache[1] = 22;
        computed_max = 1;
    }

    /* extend cache if necessary */
    if ((idx_t)n > computed_max) {
        size_t new_size = (size_t)n + 1;
        void *tmp = realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed – return 0 */
        cache = (seq_t *)tmp;
        cache_size = new_size;

        /* compute new terms using the recurrence */
        for (size_t i = (size_t)computed_max + 1; i <= (size_t)n; ++i) {
            seq_t prev1 = cache[i - 1];          /* a(i-1) */
            seq_t prev2 = cache[i - 2];          /* a(i-2) */
            seq_t coeff = (seq_t)(i - 1);          /* (n+1) in the recurrence */
            cache[i] = 22 * prev1 - 1922 * coeff * prev2;
        }
        computed_max = (idx_t)n;
    }

    return cache[(size_t)n];
}
