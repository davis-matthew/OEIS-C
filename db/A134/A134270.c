/*
 * A134270  a(n)=2a(n-1)+a(n-2)-4a(n-4).
 * https://oeis.org/A134270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 4*a(n-4) with a(0)=1, a(1)=1, a(2)=3,
 *            a(3)=7, a(4)=13
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;

    /* pre‑computed initial values */
    static const seq_t init[5] = { 1, 1, 3, 7, 13 };

    /* initialise cache lazily */
    if (cached_len == 0) {
        cached_len = 5;
        cache = (seq_t *)malloc(cached_len * sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i)
            cache[i] = init[i];
    }

    /* if requested term is already cached, return it */
    if ((size_t)n < cached_len)
        return cache[(size_t)n];

    /* extend cache up to n */
    size_t old_len = cached_len;
    cached_len = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, cached_len * sizeof(seq_t));
    for (size_t i = old_len; i <= n; ++i) {
        if (i < 5) {          /* base cases are already set but keep for safety */
            cache[i] = init[i];
            continue;
        }
        /* recurrence: a(n) = 2*a(n-1) + a(n-2) - 4*a(n-4) */
        cache[i] = 2 * cache[i-1] + cache[i-2] - 4 * cache[i-4];
    }
    return cache[(size_t)n];
}
