/*
 * A000118  Number of ways of writing n as a sum of 4 squares; also theta
 *          series of four-dimensional cubic lattice Z^4.
 * https://oeis.org/A000118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; for n > 0, a(n) = 8*A000203(n/A006519(n))*(2 +
 *            (-1)^n), i.e. 8*sigma(odd part of n), times 3 if n is even
 * Range:     u8 n <= 17, u16 n <= 2969, u32 n <= 9960, u64 n <= 9960,
 *            u128 n <= 9960, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;
    seq_t s = 0;
    idx_t d;
    for (d = 1; d <= m / d; d++) {
        if (m % d == 0) {
            idx_t e = m / d;
            s += (seq_t)d;
            if (e != d)
                s += (seq_t)e;
        }
    }
    seq_t r = s * 8;
    if ((n & 1) == 0)
        r *= 3;
    return r;
}
