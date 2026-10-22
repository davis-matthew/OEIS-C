/*
 * A006042  The nim-square of n.
 * https://oeis.org/A006042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, look
 * Formula:   a(n) = A051775(n,n), the nim-product of n with itself, computed
 *            by recursive nim multiplication on 2^k-bit halves using F^2 = F
 *            + F/2 for Fermat 2-powers F
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t nim_mul(uint64_t x, uint64_t y, int half) {
    uint64_t mask, x0, x1, y0, y1, c, d, e;
    if (half == 0) return x & y;
    mask = (half == 64) ? ~(uint64_t)0 : (((uint64_t)1 << half) - 1);
    x0 = x & mask; x1 = x >> half;
    y0 = y & mask; y1 = y >> half;
    c = nim_mul(x0, y0, half / 2);
    d = nim_mul(x1 ^ x0, y1 ^ y0, half / 2);
    e = nim_mul(nim_mul(x1, y1, half / 2), (uint64_t)1 << (half - 1), half / 2);
    return ((d ^ c) << half) | (c ^ e);
}

seq_t a(idx_t n) {
    uint64_t r = nim_mul((uint64_t)n, (uint64_t)n, 32);
    return (seq_t)r;
}
