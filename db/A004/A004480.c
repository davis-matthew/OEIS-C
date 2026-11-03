/*
 * A004480  Nim product 15 * n.
 * https://oeis.org/A004480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15 (*) n, the nim (Conway) product, computed by recursive
 *            field splitting: for X = 2^(2^k), (a1*X+a0)(*)(b1*X+b0) =
 *            (e+c)*X + (c + d(*)(X/2)), with c = a0(*)b0, d = a1(*)b1, e =
 *            (a0+a1)(*)(b0+b1), and + meaning XOR
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t nim_mul_level(uint64_t x, uint64_t y, int L) {
    if (L == 0) return x & y;
    int h = 1 << (L - 1);
    uint64_t mask = (h >= 64) ? ~(uint64_t)0 : (((uint64_t)1 << h) - 1);
    uint64_t x0 = x & mask, x1 = x >> h;
    uint64_t y0 = y & mask, y1 = y >> h;
    uint64_t c = nim_mul_level(x0, y0, L - 1);
    uint64_t d = nim_mul_level(x1, y1, L - 1);
    uint64_t e = nim_mul_level(x0 ^ x1, y0 ^ y1, L - 1);
    uint64_t low = c ^ nim_mul_level(d, (uint64_t)1 << (h - 1), L - 1);
    uint64_t high = e ^ c;
    return (high << h) | low;
}

seq_t a(idx_t n) {
    uint64_t r = nim_mul_level((uint64_t)15, (uint64_t)n, 6);
    seq_t hi = (seq_t)(idx_t)(r >> 32);
    seq_t lo = (seq_t)(idx_t)(r & 0xFFFFFFFFULL);
    return hi * (seq_t)65536 * (seq_t)65536 + lo;
}
