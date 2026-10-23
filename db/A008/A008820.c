/*
 * A008820  Expansion of (1+2*x^7+x^12)/((1-x^2)^2*(1-x^12)).
 * https://oeis.org/A008820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) - a(n-4) + a(n-12) - 2*a(n-14) + a(n-16) initial
 *            values: a(0)=1, a(1)=0, a(2)=2, a(3)=0, a(4)=3, a(5)=0, a(6)=4,
 *            a(7)=2, a(8)=5, a(9)=4, a(10)=6, a(11)=6, a(12)=9, a(13)=8,
 *            a(14)=12, a(15)=10
 * Range:     u8 n <= 77, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

extern seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with first 16 terms */
    if (!cache) {
        cache_len = 16;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1; cache[1] = 0; cache[2] = 2; cache[3] = 0;
        cache[4] = 3; cache[5] = 0; cache[6] = 4; cache[7] = 2;
        cache[8] = 5; cache[9] = 4; cache[10] = 6; cache[11] = 6;
        cache[12] = 9; cache[13] = 8; cache[14] = 12; cache[15] = 10;
    }

    /* if already computed return */
    if (n < cache_len) return cache[n];

    /* extend cache */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v = 2 * cache[i - 2] - cache[i - 4]
                  + cache[i - 12] - 2 * cache[i - 14] + cache[i - 16];
        cache[i] = v;
    }
    cache_len = new_len;
    return cache[n];
}
