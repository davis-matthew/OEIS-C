/*
 * A007516  (2^2^...^2) (n times) + 1.
 * https://oeis.org/A007516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = 2^(a(n)-1) + 1 for n >= 1, a(0) = 1, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"

seq_t a(idx_t n) {
    static seq_t cache[6];
    static idx_t cache_len = 0;

    if (cache_len == 0) {
        cache[0] = 1;
        cache[1] = 3;
        cache_len = 2;
    }

    if (n > 5) n = 5; /* a(6) has about 2^65536 bits; it cannot be represented */

    for (idx_t i = cache_len; i <= n; i++) {
        seq_t prev = cache[i - 1];
        cache[i] = A001832_pow2((idx_t)(unsigned long long)(prev - 1)) + 1;
        cache_len = i + 1;
    }

    return cache[n];
}
