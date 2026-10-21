/*
 * A091691  a(n) = (10^(n-1)-1) * (n-10) / 9.
 * https://oeis.org/A091691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (10^(n-1)-1)*(n-10)/9 for n ≥ 1 a(0) = 1 Recurrence for n
 *            ≥ 4: a(n) = 22*a(n-1) - 141*a(n-2) + 220*a(n-3) - 100*a(n-4)
 *            (used to avoid recomputing the power of 10 for each call)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;

    /* For small n we just return known values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)(-8);
    if (n == 3) return (seq_t)(-77);

    /* Ensure cache capacity */
    if (cached_len == 0) {
        /* Allocate space for first four values */
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)(-8);
        cache[3] = (seq_t)(-77);
        cached_len = 4;
    }

    if ((size_t)n >= cached_len) {
        /* Extend cache up to n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* memory failure fallback */
        cache = tmp;
        for (size_t i = cached_len; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)(22 * cache[i - 1]
                              - 141 * cache[i - 2]
                              + 220 * cache[i - 3]
                              - 100 * cache[i - 4]);
        }
        cached_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
