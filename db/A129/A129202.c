/*
 * A129202  Denominator of 3*(3+(-1)^n) / (n+1)^2.
 * https://oeis.org/A129202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = (n+1)^2 / gcd(3*(3+(-1)^n), (n+1)^2)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = (n+1)^2 without the common factor with the numerator */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* p = n+1 */
    seq_t p = (seq_t)n + 1;

    /* numerator: 3*(3+(-1)^n) -> 12 if n even, 6 if n odd */
    seq_t num = ( (n & 1) ? (seq_t)6 : (seq_t)12 );

    /* denominator before reduction: p^2 */
    seq_t denom = p * p;

    /* common factor with the numerator */
    seq_t g = gcd(num, denom);

    /* reduced denominator is denom / g */
    return denom / g;
}
