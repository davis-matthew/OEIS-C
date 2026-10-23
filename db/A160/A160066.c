/*
 * A160066  Numerator of Hermite(n, 22/25).
 * https://oeis.org/A160066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 22/25) Recurrence: a(0) = 1, a(1) = 44,
 *            a(n) = 44*a(n-1) - 1250*(n-1)*a(n-2) for n ≥ 2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to handle out‑of‑order calls */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* if already computed, return it */
    if (n < (idx_t)cap)
        return cache[(size_t)n];

    /* grow cache so that it can hold a(n)   */
    size_t new_cap = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) abort();

    /* initialise the first terms if necessary */
    if (cap == 0) {
        cache[0] = 1;
        cap = 1;
    }
    if (cap == 1 && new_cap > 1) {
        cache[1] = 44;
        cap = 2;
    }

    /* compute missing terms up to n */
    for (size_t i = cap; i <= new_cap - 1; ++i) {
        if (i == 0) {
            cache[0] = 1;                 /* already set, but keep for clarity */
        } else if (i == 1) {
            cache[1] = 44;                /* already set */
        } else {
            seq_t prod = cache[i-2] * (seq_t)(1250 * (i - 1));
            cache[i] = 44 * cache[i-1] - prod;
        }
    }

    cap = new_cap;
    return cache[(size_t)n];
}
