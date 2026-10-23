/*
 * A021289  Decimal expansion of 1/285.
 * https://oeis.org/A021289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the nth decimal digit of 1/285
 * Range:     u8 n <= 8, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Simulate long division 1/285; remainder never exceeds 284. */
    seq_t rem   = 1;   /* numerator */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        digit = (rem * 10) / 285;
        rem   = (rem * 10) % 285;
    }
    return digit;
}
