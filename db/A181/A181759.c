/*
 * A181759  Denominators of an extended Rydberg-Ritz spectrum of hydrogen
 *          atom: 1/9 -1/n^2.
 * https://oeis.org/A181759
 *
 * Offset:    -3    (a(-3) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (n^2-9)/(9*n^2) in lowest terms, with a(0)
 *            defined as 0. For n<0 the value depends only on n^2
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Special case at the offset */
    if (n == 0)
        return (seq_t)0;

    /* Use absolute value of n for square */
    seq_t nsq = (seq_t)n * (seq_t)n;          /* n^2  */
    seq_t diff = (nsq >= (seq_t)9) ? (nsq - (seq_t)9)
                                   : ((seq_t)9 - nsq);  /* |n^2-9| */

    /* Full denominator before reduction: 9 * n^2 */
    seq_t denom_full = (seq_t)9 * nsq;

    /* Greatest common divisor of numerator and full denominator */
    seq_t g = gcd(diff, denom_full);

    /* Simplified denominator */
    return denom_full / g;
}
