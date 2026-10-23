/*
 * A132736  Row sums of triangle A132735.
 * https://oeis.org/A132736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n - 1 + [n=0] (indicator for n==0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = pow2(n) + (seq_t)n - (seq_t)1;
    if (n == 0)
        res += (seq_t)1;
    return res;
}
