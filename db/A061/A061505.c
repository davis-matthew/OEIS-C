/*
 * A061505  Leading digit of n^n.
 * https://oeis.org/A061505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = leading digit of n^n (a(0)=1, a(1)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    double dn = (double)n;
    double x = dn * log10(dn);          /* x = n*log10(n) */
    double int_part;
    double frac = modf(x, &int_part);    /* frac in [0,1) */
    double leading = pow(10.0, frac);    /* leading digit times 10^0..10^1 */
    int digit = (int)leading;
    if (digit < 1) digit = 1;
    if (digit > 9) digit = 9;
    return (seq_t)digit;
}
