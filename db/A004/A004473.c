/*
 * A004473  Nim product 8 * n.
 * https://oeis.org/A004473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051775(8,n), the nim product 8 (x) n, computed by
 *            recursive Fermat-2-power splitting: for L = 2^h,
 *            (a1*L+a0)(x)(b1*L+b0) = ((a1(x)b1) ^ (a1(x)b0) ^ (a0(x)b1))*L ^
 *            (a0(x)b0) ^ ((a1(x)b1)(x)(L/2))
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 969 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t nim_mul(uint64_t a, uint64_t b, int h) {
    if (a < 2 || b < 2) return a * b;
    uint64_t mask = (h >= 64) ? ~(uint64_t)0 : (((uint64_t)1 << h) - 1);
    uint64_t a1 = a >> h, a0 = a & mask;
    uint64_t b1 = b >> h, b0 = b & mask;
    uint64_t c = nim_mul(a0, b0, h / 2);
    uint64_t d = nim_mul(a1 ^ a0, b1 ^ b0, h / 2);
    uint64_t e = nim_mul(a1, b1, h / 2);
    uint64_t high = d ^ c;
    uint64_t low = c ^ nim_mul(e, (uint64_t)1 << (h - 1), h / 2);
    return (high << h) | low;
}

seq_t a(idx_t n) {
    uint64_t v = nim_mul((uint64_t)8, (uint64_t)n, 32);
    seq_t r = 0;
    int k;
    for (k = 63; k >= 0; k--) {
        r = r * 2;
        if ((v >> k) & 1) r += 1;
    }
    return r;
}
