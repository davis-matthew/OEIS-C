/*
 * A158981  Numerator of Hermite(n, 2/7).
 * https://oeis.org/A158981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 4*a(n-1) - 98*(n-1)*a(n-2), a(0)=1, a(1)=4
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    idx_t i, start, old_len;

    /* allocate or extend cache to at least n+1 entries */
    if (cache_len == 0) {
        /* initial cache of size at least 2 */
        old_len = 2;
        cache_len = (n + 1 > old_len) ? n + 1 : old_len;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        start = old_len;
    } else {
        start = cache_len;
        if ((idx_t)(n + 1) > cache_len) {
            old_len = cache_len;
            cache_len = n + 1;
            seq_t *tmp = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
            if (!tmp) return 0;
            cache = tmp;
        }
    }

    /* compute missing entries */
    for (i = start; i <= n; ++i) {
        seq_t term = (seq_t)98 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = 4 * cache[i - 1] - term;
    }

    return cache[n];
}
