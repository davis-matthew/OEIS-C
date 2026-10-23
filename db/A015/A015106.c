/*
 * A015106  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=-9.
 * https://oeis.org/A015106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n+1) = Sum_{i=0..n} (-9)^i*a(i)*a(n-i), a(0)=1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t computed = 0;

    /* Ensure the cache can hold values up to index n */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = cache_size ? cache_size * 2 : 1;
        while (new_size <= (size_t)n + 1) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute all missing terms up to n */
    for (size_t k = computed; k <= (size_t)n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
        } else {
            seq_t sum  = (seq_t)0;
            seq_t pow9 = (seq_t)1;   /* 9^i */
            int   sign = 1;          /* (-1)^i */

            for (size_t i = 0; i < k; ++i) {
                seq_t prod = cache[i] * cache[k - 1 - i];
                if (sign < 0) prod = -prod;
                sum += prod * pow9;
                pow9 *= (seq_t)9;
                sign = -sign;
            }
            cache[k] = sum;
        }
    }

    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
