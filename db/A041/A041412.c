/*
 * A041412  Numerators of continued fraction convergents to sqrt(221).
 * https://oeis.org/A041412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 3330*a(n-6) - a(n-12) Initial values: a(0)=14, a(1)=15,
 *            a(2)=104, a(3)=223, a(4)=1442, a(5)=1665, a(6)=48062,
 *            a(7)=49727, a(8)=346424, a(9)=742575, a(10)=4801874,
 *            a(11)=5544449
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Pre‑computed first 12 terms */
    static const seq_t init[12] = {
        14, 15, 104, 223, 1442, 1665,
        48062, 49727, 346424, 742575, 4801874, 5544449
    };

    /* Cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;

    size_t needed = (size_t)(n + 1);

    /* Grow cache if necessary */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 12) {
            cache[i] = init[i];
        } else {
            cache[i] = 3330 * cache[i - 6] - cache[i - 12];
        }
    }
    if (needed > cache_len) cache_len = needed;

    return cache[n];
}
