/*
 * A021473  Decimal expansion of 1/469.
 * https://oeis.org/A021473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit at position n of decimal expansion of 1/469
 *            computed by simulating the long division of 1 by 469
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1ULL;          /* current remainder */
    for (idx_t i = 0; i <= n; ++i)
    {
        uint64_t d = (rem * 10ULL) / 469ULL;
        if (i == n)
            return (seq_t)d;
        rem = (rem * 10ULL) % 469ULL;
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
