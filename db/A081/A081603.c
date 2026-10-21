/*
 * A081603  Number of 2's in ternary representation of n.
 * https://oeis.org/A081603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digits equal to 2 in the ternary representation
 *            of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t cnt = 0;
    idx_t x = n;
    while (x)
    {
        if ((x % 3) == 2)
            ++cnt;
        x /= 3;
    }
    return cnt;
}
