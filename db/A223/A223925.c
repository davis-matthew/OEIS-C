/*
 * A223925  a(2n+1) = 2*n-1; a(2n)= 4^n.
 * https://oeis.org/A223925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(2n) = 4^n, a(2n-1) = 2n-1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* outside the defined offset */

    return (n & 1) ? (seq_t)n : pow4(n >> 1);
}
