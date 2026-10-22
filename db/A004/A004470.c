/*
 * A004470  Nim product 5 * n.
 * https://oeis.org/A004470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051775(5,n), nim product computed by recursive
 *            splitting: for X = 2^L, (a1*X + a0) (x) (b1*X + b0) = (e^c)*X ^
 *            c ^ (d (x) 2^(L-1)), c = a0(x)b0, d = a1(x)b1, e =
 *            (a0^a1)(x)(b0^b1)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

static uint64_t nim_mul_w(uint64_t a, uint64_t b, int w) {
    /* a, b < 2^w, w a power of 2 */
    if (w == 1) return a & b;
    if (a == 0 || b == 0) return 0;
    if (a == 1) return b;
    if (b == 1) return a;
    int L = w / 2;
    uint64_t mask = (L >= 64) ? ~(uint64_t)0 : (((uint64_t)1 << L) - 1);
    uint64_t a0 = a & mask, a1 = a >> L;
    uint64_t b0 = b & mask, b1 = b >> L;
    uint64_t c = nim_mul_w(a0, b0, L);
    uint64_t d = nim_mul_w(a1, b1, L);
    uint64_t e = nim_mul_w(a0 ^ a1, b0 ^ b1, L);
    uint64_t hi = e ^ c;
    uint64_t lo = c ^ nim_mul_w(d, (uint64_t)1 << (L - 1), L);
    return (hi << L) ^ lo;
}

static uint64_t nim_mul(uint64_t a, uint64_t b) {
    uint64_t m = a | b;
    int w = 1;
    while (w < 64 && (m >> w) != 0) w *= 2;
    return nim_mul_w(a, b, w);
}

seq_t a(idx_t n) {
    uint64_t r = nim_mul(5, (uint64_t)n);
    return u128_to_seq((unsigned __int128)r);
}
