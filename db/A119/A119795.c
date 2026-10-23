/*
 * A119795  a(1) = a(2) = 1. a(n) = a(n-2) + (largest power of 2 dividing
 *          a(n-1)).
 * https://oeis.org/A119795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + (largest power of 2 dividing a(n-1)), a(1) =
 *            a(2) = 1
 * Range:     u8 n <= 105, u16 n <= 3885, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, not strictly needed but harmless */

seq_t a(idx_t n)
{
    if (n < 1)          /* out of range, unused but safe */
        return 0;

    if (n == 1 || n == 2)
        return 1;

    seq_t a_prevprev = 1;   /* a(1) */
    seq_t a_prev     = 1;   /* a(2) */
    seq_t a_curr;           /* will hold a(i) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* largest power of 2 dividing a_prev */
        seq_t power = 1;
        seq_t temp  = a_prev;
        while ((temp & 1) == 0)
        {
            temp >>= 1;
            power <<= 1;
        }

        a_curr = a_prevprev + power;
        a_prevprev = a_prev;
        a_prev = a_curr;
    }

    return a_curr;
}
