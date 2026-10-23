/*
 * A049018  Expansion of 1/((1+x)^7 - x^7).
 * https://oeis.org/A049018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * Sum_{k=0..floor(n/7)} (-1)^k * binomial(n+6,
 *            7*k+6)
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 29, i64 n <= 61, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Smallest 7 terms are known explicitly */
    static const seq_t init[7] = {
        (seq_t)1,     (seq_t)-7,   (seq_t)28,
        (seq_t)-84,   (seq_t)210,  (seq_t)-462,
        (seq_t)924
    };

    if (n < 7)
        return init[n];

    /* Recurrence coefficients of (1+x)^7 - x^7 = sum_{i=0}^6 C(7,i)x^i   */
    static const idx_t coeff[7] = {1, 7, 21, 35, 35, 21, 7};

    /* Lazily allocate and grow a cache of previously computed values   */
    static seq_t *cache  = NULL;
    static idx_t  cap    = 0;
    static idx_t  length = 0;   /* number of values currently stored */

    if (cache == NULL) {
        /* start with some reasonable capacity */
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();

        /* initialise first 7 terms */
        cache[0] = init[0]; cache[1] = init[1]; cache[2] = init[2];
        cache[3] = init[3]; cache[4] = init[4]; cache[5] = init[5];
        cache[6] = init[6];
        length = 7;
    }

    if (n >= (idx_t)cap) {
        /* grow the cache. Double the size until enough for n+1. */
        idx_t new_cap = cap;
        while (n >= new_cap) new_cap <<= 1U;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap   = new_cap;
    }

    if (n >= length) {
        /* compute all missing terms up to n */
        for (idx_t i = length; i <= n; ++i) {
            seq_t s = 0;
            s += coeff[1] * cache[i-1];
            s += coeff[2] * cache[i-2];
            s += coeff[3] * cache[i-3];
            s += coeff[4] * cache[i-4];
            s += coeff[5] * cache[i-5];
            s += coeff[6] * cache[i-6];
            cache[i] = -s;
        }
        length = n + 1;
    }

    return cache[n];
}
