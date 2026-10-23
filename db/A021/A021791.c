/*
 * A021791  Decimal expansion of 1/787.
 * https://oeis.org/A021791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth digit after the decimal point of 1/787 by long division */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* remainder of the long division; always < 787 */
    uint64_t rem = 1;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        rem *= 10U;
        seq_t digit = (seq_t)(rem / 787U);  /* digit at this step */
        rem %= 787U;
        if (i == n) {
            return digit;                 /* return the desired digit */
        }
    }

    /* Should never reach here; return 0 as a fallback */
    return (seq_t)0;
}
