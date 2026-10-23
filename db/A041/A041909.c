/*
 * A041909  Denominators of continued fraction convergents to sqrt(476).
 * https://oeis.org/A041909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 57598 * a(n-8) - a(n-16) (for n > 15) with initial values
 *            a(0)=1, a(1)=1, a(2)=5, a(3)=11, a(4)=115, a(5)=241, a(6)=1079,
 *            a(7)=1320, a(8)=56519, a(9)=57839, a(10)=287875, a(11)=633589,
 *            a(12)=6623765, a(13)=13881119, a(14)=62148241, a(15)=76029360
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* small array of the first 16 terms */
    static const seq_t init[16] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)5,
        (seq_t)11,
        (seq_t)115,
        (seq_t)241,
        (seq_t)1079,
        (seq_t)1320,
        (seq_t)56519,
        (seq_t)57839,
        (seq_t)287875,
        (seq_t)633589,
        (seq_t)6623765,
        (seq_t)13881119,
        (seq_t)62148241,
        (seq_t)76029360
    };

    /* required cache size */
    size_t need = (size_t)n + 1;
    if (need < 16) need = 16;

    /* allocate or grow the cache */
    if (need > cache_size) {
        seq_t *p = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!p) return 0;   /* allocation failure (unlikely in tests) */
        cache = p;
        cache_size = need;
    }

    /* ensure the first 16 entries are correct */
    for (size_t i = 0; i < 16; ++i)
        cache[i] = init[i];

    /* compute values for indices ≥ 16 */
    for (idx_t i = 16; i <= n; ++i) {
        /* 128‑bit intermediate to avoid overflow on 8/16/32/64‑bit seq_t */
        cache[i] = (seq_t)(((__int128)57598 * cache[i-8]) - cache[i-16]);
    }

    return cache[n];
}
