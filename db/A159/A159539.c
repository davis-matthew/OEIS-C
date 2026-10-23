/*
 * A159539  Numerator of Hermite(n, 11/17).
 * https://oeis.org/A159539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=22, a(n)=22*a(n-1)-578*(n-1)*a(n-2) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* memoization table */
    static size_t cache_len = 0;  /* number of entries stored */

    if (n < cache_len) {
        return cache[n];          /* already computed */
    }

    /* Extend cache to hold up to a(n) */
    size_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed */
    cache = tmp;

    /* Compute all missing values up to a(n) */
    for (size_t i = cache_len; i < new_len; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 22;
        } else {
            /* a(i) = 22*a(i-1) - 578*(i-1)*a(i-2) */
            cache[i] = 22 * cache[i-1]
                       - (seq_t)578 * (seq_t)(i-1) * cache[i-2];
        }
    }

    cache_len = new_len;
    return cache[n];
}
