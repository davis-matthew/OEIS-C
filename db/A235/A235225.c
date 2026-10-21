/*
 * A235225  Numbers whose sum of digits is 14.
 * https://oeis.org/A235225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer with digit sum 14
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;
    for (;;)
    {
        if (digit_sum(candidate) == (seq_t)14)
        {
            ++count;
            if (count == n)
                return candidate;
        }
        /* Advance to the next integer */
        candidate += (seq_t)1;
    }
}
