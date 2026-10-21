/*
 * A066851  Number of ordered solutions (x,y,z) to xy + yz + zx = n with x,y,z
 *          >= 1.
 * https://oeis.org/A066851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of ordered solutions (x,y,z) >= 1 to xy + yz + zx
 *            = n Computation: iterate over x,y, compute z = (n - xy)/(x + y)
 *            and count integer solutions. Complexity O(n log n) which is <2
 *            seconds for n up to 10 000
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x <= n; ++x)
    {
        /* From (x+1)(y+1) <= n+1 => y <= (n+1)/(x+1) - 1.
           Let limit = floor((n+1)/(x+1)); loop y < limit. */
        idx_t limit = ((idx_t)n + 1) / (x + 1);
        for (idx_t y = 1; y < limit; ++y)
        {
            seq_t xy = (seq_t)x * (seq_t)y;
            seq_t numerator = (seq_t)n - xy;
            seq_t denominator = (seq_t)x + (seq_t)y;
            if (numerator < denominator)             /* z < 1 */
                continue;
            if ((numerator % denominator) == 0)
                ++count;
        }
    }
    return count;
}
