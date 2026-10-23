/*
 * A021652  Decimal expansion of 1/648.
 * https://oeis.org/A021652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth digit after decimal point in the expansion of 1/648
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;      /* remainder modulo 648 */
    seq_t digit = 0;       /* digit at this step */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;          /* shift decimal place */
        digit = rem / 648;  /* next digit */
        rem %= 648;         /* new remainder */
    }
    return digit;
}
