/*
 * A097414  Initial decimal digit of n^10.
 * https://oeis.org/A097414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ⌊10^{frac(10·log10(n))}⌋ (the first decimal digit of
 *            n^10)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* the index n is guaranteed ≥ 1 */
    /* compute using long double for extra precision */
    long double x = (long double)n;
    long double t = 10.0L * log10l(x);          /* 10·log10(n) */
    long double frac = t - floorl(t);           /* fractional part */
    /* tiny epsilon to avoid rounding issues when frac is very close to 1 */
    long double leading = powl(10.0L, frac + 1e-15L);
    int digit = (int)floorl(leading);
    if (digit < 1) digit = 1;                     /* safety: digit must be 1‑9 */
    return (seq_t)digit;
}
