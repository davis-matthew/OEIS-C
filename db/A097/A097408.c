/*
 * A097408  Initial decimal digit of n^4.
 * https://oeis.org/A097408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = first decimal digit of n^4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Return the leading decimal digit of n^4.
   n is 1-based; for n >= 1 the function yields a digit 1..9. */
seq_t a(idx_t n)
{
    /* Convert n to double for logarithmic computation. */
    double logn = log10((double)n);
    /* Compute 4 * log10(n). */
    double m = 4.0 * logn;
    /* Extract fractional part of m. */
    double intpart;
    double frac = modf(m, &intpart);
    /* 10^frac gives the leading digits as a number in [1,10). */
    double t = pow(10.0, frac);
    /* Convert to integer digit, adding a tiny epsilon to guard
       against floating‑point round‑off that might push a value
       just below 10 up to 10. */
    int d = (int)(t + 1e-12);
    if (d == 10) d = 9;
    return (seq_t)d;
}
