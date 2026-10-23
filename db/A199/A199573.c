/*
 * A199573  Number of round trips of length n from any of the four vertices of
 *          the cycle graph C_4.
 * https://oeis.org/A199573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(0)=1; a(n)=0 for odd n; a(n)=2^(n-1) for even n>=2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if ((n & 1) != 0)          /* odd n */
        return (seq_t)0;
    /* even n >= 2 */
    return pow2(n - 1);
}
