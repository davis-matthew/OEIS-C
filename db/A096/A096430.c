/*
 * A096430  Numerator of (9*(n^4 - 2*n^3 + 2*n^2 - n) + 2)/(2*(2*n-1)).
 * https://oeis.org/A096430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = Numerator of (9*(n^4 - 2*n^3 + 2*n^2 - n) +
 *            2)/(2*(2*n-1))
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 175, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    seq_t x2 = x * x;           /* n^2   */
    seq_t x3 = x2 * x;          /* n^3   */
    seq_t x4 = x3 * x;          /* n^4   */

    seq_t poly = x4 - (seq_t)2 * x3 + (seq_t)2 * x2 - x;
    seq_t numer = (seq_t)9 * poly + (seq_t)2;
    seq_t denom = (seq_t)2 * ((seq_t)2 * x - (seq_t)1);

    seq_t g = gcd(numer, denom);     /* cancel common factors */
    return numer / g;                /* reduced numerator   */
}
