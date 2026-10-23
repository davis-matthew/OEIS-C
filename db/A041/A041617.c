/*
 * A041617  Denominators of continued fraction convergents to sqrt(327).
 * https://oeis.org/A041617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 434*a(n-2) - a(n-4), a(0)=1, a(1)=12, a(2)=433, a(3)=5208
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
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

    size_t idx = (size_t)n;

    /* Grow cache if necessary */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to idx */
    if (cache_len <= idx) {
        /* Initialize base cases lazily */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        if (cache_len <= 1 && idx >= 1) {
            cache[1] = (seq_t)12;
            cache_len = 2;
        }
        if (cache_len <= 2 && idx >= 2) {
            cache[2] = (seq_t)433;
            cache_len = 3;
        }
        if (cache_len <= 3 && idx >= 3) {
            cache[3] = (seq_t)5208;
            cache_len = 4;
        }
        for (size_t i = cache_len; i <= idx; ++i) {
            cache[i] = (seq_t)434 * cache[i - 2] - cache[i - 4];
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
