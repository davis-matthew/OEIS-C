/*
 * A021847  Decimal expansion of 1/843.
 * https://oeis.org/A021847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth decimal digit of 1/843 after the decimal point
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rem   = 1;            /* current remainder (always < 843) */
    seq_t digit = 0;            /* digit to return */

    for (idx_t i = 0; i <= n; ++i) {
        rem   = rem * 10;                    /* bring next decimal digit into place */
        digit = rem / (seq_t)843;            /* this is the digit at position i */
        rem   = rem % (seq_t)843;            /* new remainder for next step */
    }
    return digit;
}
