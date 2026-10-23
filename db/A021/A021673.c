/*
 * A021673  Decimal expansion of 1/669.
 * https://oeis.org/A021673
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

/* Digit extraction for 1/669: computes the nth decimal digit after the point
   of 1/669 by long division.  a(0) is the first digit after the decimal. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* remainder of the current division; 1/669 starts with remainder 1 */
    uint16_t rem = 1;
    seq_t digit = 0;

    /* perform long division n+1 times, keeping only the current digit */
    for (idx_t i = 0; i <= n; ++i) {
        uint32_t prod = (uint32_t)rem * 10U;   /* multiplied by 10 */
        digit = (seq_t)(prod / 669U);          /* next decimal digit */
        rem   = (uint16_t)(prod % 669U);       /* remainder for next step */
    }

    return digit;
}
