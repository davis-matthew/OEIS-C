/*
 * A021591  Decimal expansion of 1/587.
 * https://oeis.org/A021591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = decimal digit at position n in the expansion of 1/587
 *            (0‑based). The digit is obtained by performing n+1 steps of the
 *            long division of 1 by 587
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Remainder after dividing by 587. Use uint32_t as 587 < 2^16. */
    uint32_t rem = 1;
    seq_t digit = 0;

    /* Perform n+1 steps of the long division to arrive at the nth digit. */
    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * 10U;                      /* bring down the next digit */
        digit = (seq_t)(rem / 587U);           /* quotient is the next decimal digit */
        rem  = rem % 587U;                     /* remainder for the next step   */
    }

    return digit;
}
