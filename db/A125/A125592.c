/*
 * A125592  Evil numbers (A001969) multiplied by 2.
 * https://oeis.org/A125592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   even_popcount_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: output 2 times the n-th evil number
   (a number whose binary representation has an even number of 1's).
   a(1) = 0, a(2) = 6, a(3) = 10, … */
#include <stdint.h>
#include "../utils/even_popcount_uint64.h"

seq_t a(idx_t n)
{
    uint64_t x = 0;           /* candidate number */
    idx_t    evils = 0;       /* how many evil numbers found so far */

    while (1)
    {
        if (even_popcount_uint64(x))
        {
            ++evils;
            if (evils == n)
                return (seq_t)( (seq_t)x * 2 );
        }
        ++x;
    }
}
