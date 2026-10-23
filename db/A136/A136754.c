/*
 * A136754  Leading digit of n! in base 3.
 * https://oeis.org/A136754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = leading digit of n! in base 3. Compute the fractional
 *            part of log_3(n!) and compare it with log_3(2)
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Sum of log10(i) for i = 1..n  (sum = log10(n!)) */
    long double sum = 0.0L;
    for (idx_t i = 1; i <= n; ++i)
        sum += log10l((long double)i);

    const long double log10_3 = log10l(3.0L);
    const long double log10_2 = log10l(2.0L);
    /* log_3(n!) = sum / log10_3  */
    const long double log3_factorial = sum / log10_3;
    const long double frac = log3_factorial - floorl(log3_factorial); /* fractional part */

    /* log_3(2) = log10_2 / log10_3 */
    const long double threshold = log10_2 / log10_3;

    return (frac < threshold) ? (seq_t)1 : (seq_t)2;
}
