/*
 * A021579  Decimal expansion of 1/575.
 * https://oeis.org/A021579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit of decimal expansion of 1/575 at position n (a(0)
 *            is the first digit after the decimal point)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t rem = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        /* Use 32‑bit arithmetic to avoid overflow when seq_t is 8‑bit */
        uint32_t tmp = (uint32_t)rem * 10U;
        digit = (seq_t)(tmp / 575U);
        rem   = (seq_t)(tmp % 575U);
    }
    return digit;
}
