/*
 * A130599  Transformation of sequence 3^k by sandwiching it between 1's.
 * https://oeis.org/A130599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 1 if n is even * a(n) = 3^((n-1)/2) if n is odd
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 42, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)          /* even indices */
        return 1;              /* seq_t(1) by implicit conversion */
    return pow3((n - 1) >> 1); /* compute 3^((n-1)/2) */
}
