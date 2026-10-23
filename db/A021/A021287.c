/*
 * A021287  Decimal expansion of 1/283.
 * https://oeis.org/A021287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th digit after the decimal point in the decimal
 *            expansion of 1/283. a(0) is the first digit after the decimal
 *            point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t rem = 1;         /* remainder of 1/283 */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                 /* bring down next digit */
        digit = (seq_t)(rem / 283);/* quotient is the next decimal digit */
        rem  = rem % 283;          /* remainder for next step */
    }
    return digit;
}
