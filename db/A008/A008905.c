/*
 * A008905  Leading digit of n!.
 * https://oeis.org/A008905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = leading digit of n!
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* a(0) and a(1) are 1 */
    if (n <= 1)
        return 1;

    double sum = 0.0;
    for (idx_t i = 2; i <= n; ++i)
        sum += log10((double)i);

    double frac = sum - floor(sum);
    double p = pow(10.0, frac);
    int d = (int)p;

    /* Correct a possible rare rounding error */
    if (d == 0)
        d = 1;
    else if (d >= 10)
        d = 9;

    return (seq_t)d;
}
