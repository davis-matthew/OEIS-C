/*
 * A004475  Nim product 10 * n.
 * https://oeis.org/A004475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051775(10,n), the nim product 10 (x) n, computed by the
 *            Fermat-2-power recursion: for x = 2^h, (a1*x + a0)(x)(b1*x + b0)
 *            has high half (a0^a1)(x)(b0^b1) ^ a0(x)b0 and low half a0(x)b0 ^
 *            (a1(x)b1)(x)2^(h-1)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 986 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t nim_mul_bits(uint64_t a, uint64_t b, int L) {
    if (a < 2 || b < 2) return a * b;
    if (L == 1) return a & b;
    int h = L / 2;
    uint64_t mask = (h >= 64) ? ~(uint64_t)0 : (((uint64_t)1 << h) - 1);
    uint64_t a0 = a & mask, a1 = a >> h;
    uint64_t b0 = b & mask, b1 = b >> h;
    uint64_t c = nim_mul_bits(a0, b0, h);
    uint64_t d = nim_mul_bits(a1, b1, h);
    uint64_t e = nim_mul_bits(a0 ^ a1, b0 ^ b1, h);
    uint64_t high = e ^ c;
    uint64_t low = c ^ nim_mul_bits(d, (uint64_t)1 << (h - 1), h);
    return (high << h) | low;
}

seq_t a(idx_t n) {
    uint64_t r = nim_mul_bits((uint64_t)10, (uint64_t)n, 64);
    return (seq_t)r;
}
