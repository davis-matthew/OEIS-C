/*
 * A160038  Numerator of Hermite(n, 12/25).
 * https://oeis.org/A160038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 12/25) Recurrence: a(0) = 1, a(1) = 24
 *            a(n) = 24*a(n-1) - 1250*(n-1)*a(n-2) for n ≥ 2
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;       /* number of filled entries */

    /* ensure array large enough for n+1 entries */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;                  /* a(0) */
        cache[1] = 24;                 /* a(1) */
        cache_size = 2;
    } else if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if already computed, return */
    if (n < cache_size) return cache[n];

    /* compute new values up to n */
    for (size_t j = cache_size; j <= (size_t)n; ++j) {
        if (j == 0) {
            cache[0] = 1;
        } else if (j == 1) {
            cache[1] = 24;
        } else {
            seq_t term = 1250 * (seq_t)(j - 1);   /* 1250*(n-1) */
            term *= cache[j - 2];
            cache[j] = 24 * cache[j - 1] - term;
        }
    }
    cache_size = (size_t)n + 1;
    return cache[n];
}
