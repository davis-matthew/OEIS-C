/*
 * A004382  Binomial coefficient C(8n, n-1).
 * https://oeis.org/A004382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(8*n, n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t gcd_u64(uint64_t x, uint64_t y) {
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    idx_t m = 8 * n;
    idx_t k = n - 1;
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= k; i++) {
        /* r = r * (m - k + i) / i, reduced so intermediates stay <= result */
        idx_t num = m - k + i;
        uint64_t rm = (uint64_t)(r % (seq_t)i);
        uint64_t g = gcd_u64(rm == 0 ? i : rm, i);
        uint64_t den = i / g;
        r /= (seq_t)g;
        r *= (seq_t)(num / den);
    }
    return r;
}
