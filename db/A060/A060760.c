/*
 * A060760  a(n) = 8^(n^2).
 * https://oeis.org/A060760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;          /* 8^0 = 1 */
    seq_t base   = 8;          /* base = 8 */
    uint64_t exp = (uint64_t)n * (uint64_t)n;  /* exponent = n^2 (fits for practical n) */

    while (exp)
    {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}
