/*
 * A021066  Decimal expansion of 1/62.
 * https://oeis.org/A021066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 62) / 62) (decimal digit of 1/62
 *            after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t rem = 1;              /* current remainder of 1/62 in long division */
    seq_t digit = 0;            /* the digit at position n */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                         /* shift decimal point to the right */
        digit = rem / 62;                  /* integer part is the current digit */
        rem   = rem % 62;                  /* update remainder for next step */
    }
    return digit;
}
