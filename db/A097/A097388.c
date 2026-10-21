/*
 * A097388  2n-th derivative of the Gaussian exp(-x^2) evaluated at x=0.
 * https://oeis.org/A097388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (2n)! / n! Recurrence: a(0) = 1, a(n) = -a(n-1)
 *            * 2 * (2n-1) for n ≥ 1 This uses only integer addition,
 *            subtraction, multiplication and negation, fully compatible with
 *            C11 and C++17
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;    /* array of a(0)...a(max_cached) */
    static idx_t max_cached = 0;   /* highest n for which cache[n] is valid */

    /* ensure cache[0] exists */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure – unlikely */
        cache[0] = (seq_t)1;               /* a(0) = 1 */
        max_cached = 0;
    }

    /* if requested n already computed, return it */
    if (n <= max_cached)
        return cache[(size_t)n];

    /* extend cache to accommodate up to n */
    size_t new_size = (size_t)n + 1;
    if (new_size > (size_t)max_cached + 1) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failure – unlikely */
        cache = new_cache;
    }

    /* iteratively compute missing terms */
    for (idx_t m = max_cached + 1; m <= n; ++m) {
        /* multiplier = 2 * (2*m - 1) */
        seq_t multiplier = (seq_t)2;
        multiplier *= (seq_t)(2 * m - 1);

        /* a(m) = - a(m-1) * multiplier */
        cache[(size_t)m] = -cache[(size_t)(m - 1)] * multiplier;
    }

    max_cached = n;
    return cache[(size_t)n];
}
