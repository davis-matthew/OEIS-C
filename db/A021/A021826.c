/*
 * A021826  Decimal expansion of 1/822.
 * https://oeis.org/A021826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = (floor(10^(n+1) / 822)) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t denom = 822ULL;
    uint64_t rem = 1ULL;
    uint64_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t tmp = rem * 10ULL;
        digit = tmp / denom;
        rem   = tmp % denom;
    }
    return (seq_t)digit;
}
