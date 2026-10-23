/*
 * A159620  Numerator of Hermite(n, 3/19).
 * https://oeis.org/A159620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy
 * Formula:   a(0)=1, a(1)=6, a(n)=6*a(n-1)-722*(n-1)*a(n-2) for n≥2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        if (cache)
            cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        else
            cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = old_len; i < cache_len; ++i) {
            if (i == 0)
                cache[i] = 1;
            else if (i == 1)
                cache[i] = 6;
            else {
                seq_t tmp = cache[i - 2] * (seq_t)722 * (seq_t)(i - 1);
                cache[i] = (seq_t)6 * cache[i - 1] - tmp;
            }
        }
    }
    return cache[n];
}
