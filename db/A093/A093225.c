/*
 * A093225  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 25.
 * https://oeis.org/A093225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10^(2n) - 25 * 10^n (a(1) = 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)               /* a(1) = 0, a(0) is not defined in OEIS */
        return (seq_t)0;

    /* Compute 10^n in pow10n */
    seq_t pow10n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow10n *= (seq_t)10;

    /* 10^(2n) = (10^n)^2 */
    seq_t pow10_2n = pow10n * pow10n;

    /* a(n) = 10^(2n) - 25 * 10^n */
    return pow10_2n - (seq_t)25 * pow10n;
}
