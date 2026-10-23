/*
 * A021230  Decimal expansion of 1/226.
 * https://oeis.org/A021230
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

/* Computes the n-th digit of the decimal expansion of 1/226 after the decimal point.
   a(0) is the first digit after the decimal point.  The digit is obtained by
   successive long‑division steps.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1/226 has a period of 112; the digits are produced by long division. */
    const idx_t divisor = 226;
    idx_t remainder = 1;          /* start with 1 before decimal point */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        remainder *= 10;          /* bring down next digit = 0 */
        digit = remainder / divisor;     /* next digit of the expansion */
        remainder = remainder % divisor; /* keep the new remainder */
    }

    return digit;
}
