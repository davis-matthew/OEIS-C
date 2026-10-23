/*
 * A021172  Decimal expansion of 1/168.
 * https://oeis.org/A021172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/168) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    const seq_t denom = (seq_t)168;
    seq_t rem = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= (seq_t)10;
        digit = rem / denom;
        rem %= denom;
    }
    return digit;
}
