/*
 * A021308  Decimal expansion of 1/304.
 * https://oeis.org/A021308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/304, computed by
 *            simulated long division. The remainder is initialized to 1; each
 *            time it is multiplied by 10, the digit is the integer part of
 *            the division by 304, and the new remainder is the modulus by
 *            304. This yields the n-th decimal digit (0‑based)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Remainder always stays smaller than 304. */
    idx_t rem = 1;

    /* Advance the remainder up to the (n)-th digit. */
    for (idx_t i = 0; i < n; ++i) {
        rem = (rem * 10) % 304;
    }

    /* Compute the nth digit. */
    idx_t digit = (rem * 10) / 304;
    return (seq_t)digit;
}
