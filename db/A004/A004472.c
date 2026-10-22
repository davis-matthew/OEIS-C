/*
 * A004472  Nim product 7 * n.
 * https://oeis.org/A004472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051775(7,n), the Nim product 7 (x) n, computed by the
 *            recursive Fermat-2-power splitting: (aH+b)(x)(cH+d) =
 *            ((a^b)(x)(c^d) ^ b(x)d) H ^ b(x)d ^ (a(x)c)(x)(H/2), with H =
 *            2^(2^(L-1))
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t nim_mul(uint64_t x, uint64_t y, int L) {
    if (x == 0 || y == 0) return 0;
    if (L == 0) return x & y;
    int h = 1 << (L - 1);
    uint64_t m = (h == 64) ? ~(uint64_t)0 : (((uint64_t)1 << h) - 1);
    uint64_t a = x >> h, b = x & m, c = y >> h, d = y & m;
    uint64_t ac = nim_mul(a, c, L - 1);
    uint64_t bd = nim_mul(b, d, L - 1);
    uint64_t t = nim_mul(a ^ b, c ^ d, L - 1);
    uint64_t high = t ^ bd;
    uint64_t low = bd ^ nim_mul(ac, (uint64_t)1 << (h - 1), L - 1);
    return (high << h) | low;
}

seq_t a(idx_t n) {
    uint64_t x = (uint64_t)n;
    int L = 0;
    while (L < 6 && (x >> (1 << L)) != 0) L++;
    if (L < 2) L = 2; /* 7 needs at least 4 bits */
    uint64_t r = nim_mul(7, x, L);
    seq_t res = (seq_t)(uint32_t)(r >> 32);
    res <<= 32;
    res |= (seq_t)(uint32_t)(r & 0xFFFFFFFFu);
    return res;
}
