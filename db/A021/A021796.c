/*
 * A021796  Decimal expansion of 1/792.
 * https://oeis.org/A021796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = floor(10^(n+1) / 792) mod 10
 *     = digit of the decimal expansion of 1/792 at position n+1 after the decimal point
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t remainder = 1;   /* numerator of 1/792 */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        remainder = remainder * 10;     /* shift decimal point */
        digit = remainder / 792;        /* next digit */
        remainder = remainder % 792;    /* remainder for next step */
    }
    return digit;
}
