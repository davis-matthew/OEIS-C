/*
 * A004323  Binomial coefficient C(3n,n-5).
 * https://oeis.org/A004323
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n, n-5) = Product_{i=1..n-5} (2n+5+i)/i
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 49,
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
    idx_t k = n - 5;
    idx_t base = 2 * n + 5;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = g_gcd((uint64_t)i, rm);
        uint64_t d = (uint64_t)i / g;
        seq_t num = (seq_t)(base + i) / (seq_t)d;
        r = (r / (seq_t)g) * num;
    }
    return r;
}
