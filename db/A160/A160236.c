/*
 * A160236  Numerator of Hermite(n, 5/29).
 * https://oeis.org/A160236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=10;
   a(n) = 10*a(n-1) - 1682*(n-1)*a(n-2)  for n >= 2
   This implements a(n) = 29^n * Hermite(n, 5/29). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* allocated size */
    static idx_t computed = 0;    /* highest index computed */

    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        computed = 1;
    }

    if (n > computed) {
        if (n + 1 > cache_len) {
            cache_len = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t factor = (seq_t)1682 * (seq_t)(i - 1);
            cache[i] = (seq_t)10 * cache[i - 1] - factor * cache[i - 2];
        }
        computed = n;
    }

    return cache[n];
}
