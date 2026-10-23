/*
 * A101626  Initial decimal digit of n^12.
 * https://oeis.org/A101626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(10^frac(12*log10(n))) (n ≥ 1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* only defined for n ≥ 1 */

    double dn = (double)n;               /* convert to floating point */
    double exponent = 12.0 * log10(dn);  /* 12·log10(n) */
    double frac = exponent - floor(exponent);   /* fractional part */

    double p10 = pow(10.0, frac);        /* 10^frac */
    int digit = (int)p10;                /* first decimal digit */

    if (digit == 10) digit = 1;          /* safety against rounding to 10 */
    return (seq_t)digit;                 /* cast to seq_t */
}
