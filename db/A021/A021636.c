/*
 * A021636  Decimal expansion of 1/632.
 * https://oeis.org/A021636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/632) - 10*floor(10^n/632) (nth decimal
 *            digit of the fraction 1/632, offset 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t denom = 632;
    uint64_t rem = 1;
    for (idx_t i = 0; i < n; ++i) {
        rem = (rem * 10) % denom;
    }
    return (seq_t)((rem * 10) / denom);
}
