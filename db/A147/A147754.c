/*
 * A147754  Terms of this sequence are equal to gcd between two polynomials
 *          P1(n)=(512*n^4+1024*n^3+712*n^2+194*n+15) and
 *          P2(n)=(120*n^2+151*n+47) which are used in the BBP formula.
 * https://oeis.org/A147754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(512*n^4 + 1024*n^3 + 712*n^2 + 194*n + 15, 120*n^2 +
 *            151*n + 47)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nn   = n;
    seq_t n2   = nn * nn;
    seq_t n3   = n2 * nn;
    seq_t n4   = n3 * nn;

    seq_t p1 = (512 * n4) + (1024 * n3) + (712 * n2) + (194 * nn) + 15;
    seq_t p2 = (120 * n2) + (151 * nn) + 47;

    return gcd(p1, p2);
}
