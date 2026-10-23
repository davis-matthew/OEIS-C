/*
 * A021848  Decimal expansion of 1/844.
 * https://oeis.org/A021848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the nth decimal digit of 1/844, computed by long
 *            division
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t remainder = 1;   /* start with the numerator 1 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        remainder = remainder * (seq_t)10;
        digit = remainder / (seq_t)844;
        remainder = remainder % (seq_t)844;
    }
    return digit;
}
