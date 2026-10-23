/*
 * A021056  Decimal expansion of 1/52.
 * https://oeis.org/A021056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 52) mod 10 (nth digit after the decimal
 *            point in the expansion of 1/52)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned long long denom = 52ULL;
    unsigned long long r = 1ULL;           /* remainder before the current digit */
    unsigned long long digit = 0ULL;       /* current digit */

    for (idx_t i = 0; i <= n; ++i) {
        unsigned long long temp = r * 10ULL;
        digit = temp / denom;
        r = temp % denom;
    }
    return (seq_t)digit;
}
