/*
 * A097410  Initial decimal digit of n^6.
 * https://oeis.org/A097410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(10^fractional_part(6 * log10(n)))
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* n >= 1, so log10 is well‑defined */
    long double x = 6.0L * log10((long double)n);
    long double frac = x - floorl(x);          /* fractional part of 6*log10(n) */
    long double val = powl(10.0L, frac) + 1e-12L;  /* guard against round‑off */
    int digit = (int)val;                      /* 1 <= digit <= 9 */
    if (digit == 10) digit = 9;                /* safety for pathological rounding */
    return (seq_t)digit;
}
