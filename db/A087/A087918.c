/*
 * A087918  Let A(n) be the matrix in the group GL(n,2) such that for 1<=i,
 *          j<=n: A[i,j] = 1 if i+j = n+1 A[i,j] = 0 if i+j != n+1. a(n) is
 *          the size of the conjugacy class of A(n) in GL(n,2).
 * https://oeis.org/A087918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{ ⌊n/2⌋(⌊n/2⌋-1)/2 } * ∏_{j=⌊n/2⌋+1}^{n} (2^j - 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* m = floor(n/2) */
    idx_t m = n / 2;
    seq_t result = 1;

    /* multiply by (2^j - 1) for j = m+1 .. n */
    for (idx_t j = m + 1; j <= n; ++j) {
        seq_t term = pow2(j) - 1;
        result *= term;
    }

    /* multiply by 2^{ m(m-1)/2 } */
    idx_t e = m * (m - 1) / 2;
    result *= pow2(e);

    return result;
}
