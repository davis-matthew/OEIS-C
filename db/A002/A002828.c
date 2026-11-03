/*
 * A002828  Least number of squares that add up to n.
 * https://oeis.org/A002828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = 0; a(n) = 1 if n is a square, else 2 if n is a sum of two
 *            squares (every prime p == 3 mod 4 divides n to an even power),
 *            else 4 if n = 4^a*(8b+7), else 3 (Legendre's three-square
 *            theorem and Lagrange's four-square theorem)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 122 terms through n = 9623 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

static int s2828_two_squares(uint64_t m) {
    uint64_t p;
    while (m % 2 == 0) m /= 2;
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (p % 4 == 3 && (e & 1)) return 0;
        }
    }
    if (m > 1 && m % 4 == 3) return 0;
    return 1;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n, r;
    if (m == 0) return (seq_t)0;
    r = isqrt_u64(m);
    if (r * r == m) return (seq_t)1;
    if (s2828_two_squares(m)) return (seq_t)2;
    while (m % 4 == 0) m /= 4;
    if (m % 8 == 7) return (seq_t)4;
    return (seq_t)3;
}
