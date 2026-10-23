/*
 * A048496  a(n) = 2^(n-1)*(3*n-4) + 3.
 * https://oeis.org/A048496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(3*n-4) + 3, a(0)=1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t p = pow2(n - 1);
    seq_t t = (seq_t)3 * (seq_t)n - (seq_t)4;
    return p * t + (seq_t)3;
}
