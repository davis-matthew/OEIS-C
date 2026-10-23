/*
 * A021467  Decimal expansion of 1/463.
 * https://oeis.org/A021467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0) = 0 (the integer part of 1/463). For n >= 1, a(n) is the
 *            nth digit after the decimal point in the expansion of 1/463.
 *            This is obtained by long–division with a starting remainder of
 *            10 to align with the OEIS indexing (which skips the first
 *            trivial zero after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* integer part */

    /* remainder after generating the first zero after the decimal point */
    idx_t rem = 10 % 463;           /* 10 */
    seq_t digit = 0;

    /* advance to the (n‑1)th digit (excluding the leading zero) */
    for (idx_t i = 1; i < n; ++i) {
        rem *= 10;
        rem %= 463;
    }

    /* compute the nth digit */
    rem *= 10;
    digit = rem / 463;

    return digit;
}
