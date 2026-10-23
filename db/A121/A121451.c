/*
 * A121451  Maximum product over partitions into parts of the form 3k+2.
 * https://oeis.org/A121451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1 or 3; otherwise a(n) = 2^(n/2) if n is even,
 *            a(n) = 5 * 2^((n-5)/2) if n is odd
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1 || n == 3)
        return 0;
    if ((n & 1) == 0)           /* even n */
    {
        return pow2(n >> 1);
    }
    else                         /* odd n (>=5) */
    {
        return (seq_t)5 * pow2((n - 5) >> 1);
    }
}
