/*
 * A021836  Decimal expansion of 1/832.
 * https://oeis.org/A021836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal expansion of 1/832
   a(n) = digit at index n (starting at 0) after the decimal point of 1/832.
   Implemented by simulating long division of 1 by 832.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t digit = 0;
    seq_t r = 1;             /* current remainder (mod 832)          */
    for (idx_t i = 0; i <= n; ++i)
    {
        r = r * 10;          /* bring the next decimal digit        */
        digit = r / 832;     /* quotient gives the next digit       */
        r = r % 832;         /* remainder for next step             */
    }
    return digit;
}
