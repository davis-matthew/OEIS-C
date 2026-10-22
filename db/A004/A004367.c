/*
 * A004367  Binomial coefficient C(6n,n-12).
 * https://oeis.org/A004367
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(6n, n-12) = Product_{i=1..n-12} (5n+12+i)/i
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 18, u64 n <= 25, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t g_gcd(uint64_t x, uint64_t y) {
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    uint64_t m = 6 * (uint64_t)n;
    uint64_t k = (uint64_t)n - 12;
    seq_t r = 1;
    uint64_t i;
    for (i = 1; i <= k; i++) {
        uint64_t num = m - k + i;
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = g_gcd(i, rm);
        uint64_t d = i / g;
        r /= (seq_t)g;
        r *= (seq_t)(num / d);
    }
    return r;
}
