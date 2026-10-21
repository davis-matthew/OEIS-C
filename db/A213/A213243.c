/*
 * A213243  Number of nonzero elements in GF(2^n) that are cubes.
 * https://oeis.org/A213243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 1) / gcd(2^n - 1, 3) Equivalent piecewise: if n is
 *            odd, a(n) = 2^n - 1; if n is even, a(n) = (2^n - 1) / 3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t m = pow2(n);
    m -= (seq_t)1;
    if ((n & 1ULL) == 0)
        m /= (seq_t)3;
    return m;
}
