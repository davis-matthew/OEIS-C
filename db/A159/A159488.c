/*
 * A159488  Numerator of Hermite(n, 1/13).
 * https://oeis.org/A159488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13^n * Hermite(n, 1/13) Recurrence: a(0) = 1, a(1) = 2
 *            a(n) = 2*a(n-1) - 338*(n-1)*a(n-2) for n >= 2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of terms currently stored */

    /* initialise cache with a(0) = 1 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;
    }

    /* ensure a(1) is stored when needed */
    if (cache_len == 1 && n >= 1) {
        cache_len = 2;
        cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        cache[1] = (seq_t)2;
    }

    /* if the requested term is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* extend the cache up to n inclusive */
    cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];      /* a(i-1) */
        seq_t prev2 = cache[i - 2];      /* a(i-2) */
        /* a(i) = 2*a(i-1) - 338*(i-1)*a(i-2) */
        seq_t term = (seq_t)2 * prev1 -
                     (seq_t)338 * (seq_t)(i - 1) * prev2;
        cache[i] = term;
    }
    cache_len = n + 1;
    return cache[n];
}
