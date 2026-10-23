/*
 * A021422  Decimal expansion of 1/418.
 * https://oeis.org/A021422
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

/* Computation: digits of 1/418 via long division (decimal expansion of 1/418) */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i, rem = 1;          /* remainder after the previous digit */
    seq_t digit = 0;           /* digit to return */

    for (i = 0; i <= n; ++i) {
        idx_t up = rem * 10;                    /* bring down next digit */
        digit = (seq_t)(up / 418);              /* compute current decimal digit */
        rem   = up % 418;                       /* update remainder for next step */
    }
    return digit;
}
