/*
 * A139388  Nearest integer to (log(744+(12(n^2-1))^3)/Pi)^2.
 * https://oeis.org/A139388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to (log(744+(12(n^2-1))^3)/Pi)^2
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Use long double arithmetic to maintain precision */
    long double nn = (long double)n;
    long double m  = 12.0L * (nn * nn - 1.0L);
    long double x  = 744.0L + m * m * m;
    long double pi = acosl(-1.0L);          /* Pi as a long double */
    long double v  = log(x) / pi;           /* log(x)/Pi */
    v = v * v;                              /* Square the value */
    return (seq_t)llroundl(v);              /* Round to nearest integer */
}
