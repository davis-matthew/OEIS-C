/*
 * A041751  Denominators of continued fraction convergents to sqrt(395).
 * https://oeis.org/A041751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 318*a(n-4) - a(n-8) (n > 7) a(0) = 1, a(1) = 1, a(2) = 7,
 *            a(3) = 8, a(4) = 311, a(5) = 319, a(6) = 2225, a(7) = 2544
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    /* initial terms */
    static const seq_t base[8] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)7,
        (seq_t)8,
        (seq_t)311,
        (seq_t)319,
        (seq_t)2225,
        (seq_t)2544
    };

    /* grow cache if needed */
    if (n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)n + 1;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1; /* avoid overflow */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute values up to n */
    if (n + 1 > cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 8) {
                cache[i] = base[(size_t)i];
            } else {
                cache[i] = (seq_t)318 * cache[i - 4] - cache[i - 8];
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
