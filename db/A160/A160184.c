/*
 * A160184  Numerator of Hermite(n, 1/28).
 * https://oeis.org/A160184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 14^n * Hermite(n, 1/28) Recurrence: a(0) = 1, a(1) = 1,
 *            a(n) = a(n-1) - 392*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* init cache for the first two terms */
    if (cache == NULL) {
        cache_cap = 4;  /* start with a small capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* grow cache if needed */
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            idx_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { abort(); }  /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        idx_t i = cache_len;
        seq_t prev = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t factor = (seq_t)(392 * (i - 1));     /* 392*(i-1) fits in uint64_t */
        seq_t term = prev2 * factor;
        cache[i] = prev - term;
        cache_len++;
    }
    return cache[n];
}
