/*
 * A042099  Denominators of continued fraction convergents to sqrt(573).
 * https://oeis.org/A042099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 766*a(n-4) - a(n-8) Initial values: a(0)=1, a(1)=1,
 *            a(2)=15, a(3)=16, a(4)=751, a(5)=767, a(6)=11489, a(7)=12256
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    /* Initialise cache with first 8 terms */
    if (cache == NULL) {
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)751;
        cache[5] = (seq_t)767;
        cache[6] = (seq_t)11489;
        cache[7] = (seq_t)12256;
    }

    if (n < 8) {
        return cache[(size_t)n];
    }

    /* Extend cache up to requested n */
    if ((idx_t)cache_len <= n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* If realloc fails, keep old cache and return */
            return cache[(size_t)n];
        }
        cache = tmp;
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 4] * (seq_t)766 - cache[i - 8];
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
