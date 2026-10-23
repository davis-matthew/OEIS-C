/*
 * A021183  Decimal expansion of 1/179.
 * https://oeis.org/A021183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/179) mod 10 (decimal digit of 1/179 after
 *            the point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;   /* current remainder of 1/179 */
    seq_t digit = 0;    /* digit to return */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;                     /* shift decimal place */
        digit = (seq_t)(rem / 179);    /* integer part => digit */
        rem %= 179;                    /* update remainder */
    }
    return digit;
}
