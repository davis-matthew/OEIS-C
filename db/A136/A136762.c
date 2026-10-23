/*
 * A136762  Leading digit of n! in base 12.
 * https://oeis.org/A136762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8852, u16 n <= 8852, u32 n <= 8852, u64 n <= 8852,
 *            u128 n <= 8852, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Leading digit of n! in base 12
 *
 * The leading digit d(n) of n! in base 12 is
 *   d(n) = ⌊ 12^{ frac( Σ_{k=1}^{n} log_{12} k ) } ⌋ .
 * For n = 0 or 1, the digit is 1.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    static const long double LOG10_12 = log10l(12.0L);
    long double sum = 0.0L;
    for (idx_t i = 2; i <= n; ++i)
        sum += log10l((long double)i) / LOG10_12;

    long double intpart;
    long double frac = modfl(sum, &intpart);
    if (frac < 0.0L) /* safety for negative sums, should not happen */
        frac += 1.0L;

    const long double EPS = 1e-12L;
    long double val = powl(12.0L, frac + EPS);
    long double d = floorl(val + EPS);

    unsigned int digit = (unsigned int)d;
    if (digit < 1)   digit = 1;
    if (digit > 11)  digit = 11;

    return (seq_t)digit;
}
