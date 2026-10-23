/*
 * A118392  Denominator of sum of reciprocals of first n tetrahedral numbers
 *          A000292.
 * https://oeis.org/A118392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator( 3*n*(n+3) / ( 2*(n+1)*(n+2) ) )
 * Range:     u8 n <= 7, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator of the reduced fraction of the sum of reciprocals
   of the first n tetrahedral numbers. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sn = n;
    seq_t num = (seq_t)3 * sn * (sn + (seq_t)3);
    seq_t den = (seq_t)2 * (sn + (seq_t)1) * (sn + (seq_t)2);
    seq_t g = gcd(num, den);
    return den / g;
}
