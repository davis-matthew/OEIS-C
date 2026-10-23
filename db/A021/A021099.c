/*
 * A021099  Decimal expansion of 1/95.
 * https://oeis.org/A021099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   digits of the decimal expansion of 1/95 (OEIS A021099)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    uint64_t r = 1;
    for (idx_t i = 0; i <= n; ++i)
    {
        uint64_t r10  = r * 10U;
        uint64_t digit = r10 / 95U;
        r = r10 % 95U;
        if (i == n)
            return (seq_t)digit;
    }
    return (seq_t)0; /* unreachable */
}
