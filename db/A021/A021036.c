/*
 * A021036  Decimal expansion of 1/32.
 * https://oeis.org/A021036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/32) mod 10 (the nth digit after the
 *            decimal point of 1/32)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i;
    seq_t rem = 1;      /* current remainder of 1*10^i modulo 32 */
    seq_t digit = 0;    /* the ith decimal digit of 1/32 */

    for (i = 0; i <= n; ++i) {
        rem = rem * 10;           /* shift left one decimal place */
        digit = rem / 32;         /* integer part gives the digit */
        rem = rem % 32;           /* keep only the fractional part */
    }
    return digit;
}
