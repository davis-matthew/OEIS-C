/*
 * A129197  a(n) = numerator( 3*(3+(-1)^n)/(n+1)^3 ).
 * https://oeis.org/A129197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator( 3*(3+(-1)^n)/(n+1)^3 )
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Numerator part */
    seq_t factor = ((n & (idx_t)1) == 0) ? (seq_t)4 : (seq_t)2;   /* 3+(-1)^n */
    seq_t num    = (seq_t)3 * factor;                             /* 3*(3+(-1)^n) */

    /* Denominator part */
    seq_t m   = (seq_t)(n + 1);
    seq_t den = m * m * m;     /* (n+1)^3, fits comfortably for n up to at least 10^4 */

    /* Reduce fraction by GCD */
    seq_t g = gcd(num, den);
    return num / g;
}
