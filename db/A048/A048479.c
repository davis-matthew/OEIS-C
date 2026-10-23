/*
 * A048479  a(n) = T(7,n), array T given by A048472.
 * https://oeis.org/A048479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n * 2^(n-1) + 1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
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

    seq_t res = (seq_t)8 * (seq_t)n;
    res *= pow2(n - 1);
    return res + (seq_t)1;
}
