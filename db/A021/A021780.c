/*
 * A021780  Decimal expansion of 1/776.
 * https://oeis.org/A021780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/776) mod 10 (the n‑th decimal digit of
 *            1/776 after the point, 0‑indexed, so a(0) is the first digit)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t rem = 1;          /* current remainder of long division */
    seq_t digit = 0;        /* placeholder for the last computed digit */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;              /* bring down next decimal place */
        digit = rem / 776;      /* next digit */
        rem   = rem % 776;      /* update remainder */
    }

    return digit;
}
