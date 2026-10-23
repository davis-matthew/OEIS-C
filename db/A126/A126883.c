/*
 * A126883  a(n) = (2^0)*(2^1)*(2^2)*(2^3)...(2^n)-1 = 2^T(n) - 1 where T(n) =
 *          A000217(n) is the n-th triangular number.
 * https://oeis.org/A126883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n*(n+1)/2) - 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute the n-th triangular number T(n) = n*(n+1)/2 */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;
    /* compute 2^T using the provided helper */
    seq_t p = pow2((idx_t)T);
    /* subtract 1 to obtain the sequence value */
    return p - 1;
}
