/*
 * A159658  Numerator of Hermite(n, 3/20).
 * https://oeis.org/A159658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 3*a(n-1) - 200*(n-1)*a(n-2), a(0)=1, a(1)=3
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache;

#ifdef __cplusplus
        new_cache = new seq_t[(size_t)new_size];
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
#else
        new_cache = realloc(cache, (size_t)new_size * sizeof(seq_t));
#endif
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)3;
            } else {
                cache[i] = (seq_t)3 * cache[i - 1]
                           - (seq_t)200 * (seq_t)(i - 1) * cache[i - 2];
            }
        }

        cache_size = new_size;
    }

    return cache[n];
}
