/*
 * A021109  Decimal expansion of 1/105.
 * https://oeis.org/A021109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/105) mod 10 (nth decimal digit of 1/105)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t rem = 1;                      /* remainder of the long division */
    for (idx_t i = 0; i <= n; ++i) {
        rem       *= 10;                /* bring down next digit */
        seq_t digit = (seq_t)(rem / 105); /* current decimal digit   */
        rem        = rem % 105;             /* update remainder       */
        if (i == n)
            return digit;                 /* answer for requested index */
    }
    /* Unreachable, but keeps compiler happy */
    return 0;
}
