/*
 * A004476  Nim product 11 * n.
 * https://oeis.org/A004476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051775(11,n), the nim product 11 (x) n, computed with
 *            the Fermat 2-power recursion F (x) F = F XOR F/2, where F =
 *            2^(2^k)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t nim_mul_w(uint64_t x, uint64_t y, int w) {
    int h;
    uint64_t m, a1, a0, b1, b0, c, lo, mid, hi;
    if (x < 2 || y < 2) return x * y;
    if (w == 1) return x & y;
    h = w / 2;
    if ((x >> h) == 0 && (y >> h) == 0) return nim_mul_w(x, y, h);
    m = (((uint64_t)1) << h) - 1;
    a1 = x >> h; a0 = x & m;
    b1 = y >> h; b0 = y & m;
    c = nim_mul_w(a1, b1, h);
    lo = nim_mul_w(a0, b0, h);
    mid = nim_mul_w(a0 ^ a1, b0 ^ b1, h) ^ lo ^ c;
    hi = c ^ mid;
    lo ^= nim_mul_w(c, ((uint64_t)1) << (h - 1), h);
    return (hi << h) | lo;
}

seq_t a(idx_t n) {
    uint64_t r = nim_mul_w((uint64_t)11, (uint64_t)n, 64);
    return (seq_t)r;
}
