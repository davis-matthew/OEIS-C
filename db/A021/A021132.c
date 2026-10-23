/*
 * A021132  Decimal expansion of 1/128.
 * https://oeis.org/A021132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A021132 – Decimal expansion of 1/128.
    a(n) is the nth digit of the decimal representation of 1/128
    (a(0)=0 is the integer part, a(1) is the first decimal digit).
    Formula: a(n) = floor(10^(n+1)/128) mod 10
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem   = 1;   /* current remainder of the long‑division */
    uint64_t digit = 0;   /* current decimal digit */

    for (idx_t i = 0; i < n + 1; ++i)
    {
        rem   *= 10;          /* shift remainder */
        digit  = rem / 128;   /* quotient = next decimal digit */
        rem   %= 128;         /* remainder for next step */
    }

    return (seq_t)digit;
}
