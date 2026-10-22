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
 * Range:     u8 n <= 9623, u16 n <= 9623, u32 n <= 9623, u64 n <= 9623,
 *            u128 n <= 9623, big any n
 * Helpers:   s2828_sqrt, s2828_two_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/s2828_sqrt.h"
#include "../utils/s2828_two_squares.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n, r;
    if (m == 0) return (seq_t)0;
    r = s2828_sqrt(m);
    if (r * r == m) return (seq_t)1;
    if (s2828_two_squares(m)) return (seq_t)2;
    while (m % 4 == 0) m /= 4;
    if (m % 8 == 7) return (seq_t)4;
    return (seq_t)3;
}
