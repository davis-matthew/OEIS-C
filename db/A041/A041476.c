/*
 * A041476  Numerators of continued fraction convergents to sqrt(254).
 * https://oeis.org/A041476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 510*a(n-4) - a(n-8) Initial values: a(0)=15, a(1)=16,
 *            a(2)=239, a(3)=255, a(4)=7889, a(5)=8144, a(6)=121905,
 *            a(7)=130049
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
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
    static size_t cache_cap = 0;
    static const seq_t init[8] = {15, 16, 239, 255, 7889, 8144, 121905, 130049};

    if (n < 8)
        return init[n];

    if ((idx_t)cache_len <= n) {
        /* Ensure capacity */
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* Copy initial values if not already cached */
        if (cache_len < 8) {
            for (size_t i = cache_len; i < 8; ++i)
                cache[i] = init[i];
            cache_len = 8;
        }
        /* Compute missing terms up to n */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t val4 = cache[i - 4];
            seq_t val8 = cache[i - 8];
            cache[i] = (seq_t)510 * val4 - val8;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
