/*
 * A021643  Decimal expansion of 1/639.
 * https://oeis.org/A021643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/639) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k;
    uint64_t r = 1;
    seq_t digit = 0;
    for (k = 0; k <= n; ++k) {
        r *= 10;
        digit = r / 639;
        r %= 639;
    }
    return digit;
}
