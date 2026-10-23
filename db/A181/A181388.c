/*
 * A181388  a(n) = Sum_{k=1..n} 2^T(k-1), where T = A000217 are the triangular
 *          numbers 0, 1, 3, 6, 10, ... . For n=0 we have the empty sum equal
 *          to 0.
 * https://oeis.org/A181388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^T(k-1), where T(k) = k*(k+1)/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sum = 0;
    seq_t pow = 1;          /* 2^T(0) = 1 */
    for (idx_t i = 1; i <= n; ++i)
    {
        sum += pow;             /* add 2^T(i-1) */
        pow *= pow2(i);         /* pow = 2^T(i) = pow * 2^i */
    }
    return sum;
}
