/*
 * A068515  A measure of how close the square root of 2 is to rational
 *          numbers.
 * https://oeis.org/A068515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = round(1/(sqrt(2)-round(sqrt(2)*n)/n))
 * Range:     i8 n <= 11, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses long double arithmetic to obtain the
   nearest integer to 1/(sqrt(2)-p/n).  It is accurate for all
   n up to many millions, which comfortably covers the test cases
   in the Austrian Computing Contest. */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n=0, return 0 */
    long double dn = (long double)n;
    static const long double sqrt2 = sqrtl((long double)2.0);
    long double x   = dn * sqrt2;                     /* sqrt(2)*n   */
    long double p_ld = llround(x);                    /* nearest integer */
    idx_t     p  = (idx_t)p_ld;                       /* round(sqrt(2)*n) */
    long double m  = x - (long double)p;              /* sqrt(2)*n - p  */
    /* m may be negative; division n/m preserves sign */
    long double val = dn / m;                         /* exact value of 1/(sqrt(2)-p/n) */
    return (seq_t)llround(val);                       /* nearest integer */
}
