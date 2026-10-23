/*
 * A134022  Number of negative trits in balanced ternary representation of n.
 * https://oeis.org/A134022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of -1 digits in the balanced ternary
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t m = n;
    while (m > 0)
    {
        idx_t r = m % 3u;
        idx_t q = m / 3u;
        if (r == 2u)
        {
            count++;
            m = q + 1;
        }
        else
        {
            m = q;
        }
    }
    return count;
}
