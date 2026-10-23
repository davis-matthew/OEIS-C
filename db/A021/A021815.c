/*
 * A021815  Decimal expansion of 1/811.
 * https://oeis.org/A021815
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

/* Compute the nth digit after the decimal point of 1/811
   (decimal expansion of 1/811).  Uses long‑division. */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t rem   = 1;   /* current remainder of 1/811 */
    seq_t digit = 0;   /* digit at current step */

    /* Perform n+1 steps; the final digit is a(n). */
    for (idx_t i = 0; i <= n; ++i) {
        rem   = rem * (idx_t)10;
        digit = (seq_t)(rem / (idx_t)811);
        rem   = rem % (idx_t)811;
    }
    return digit;
}
