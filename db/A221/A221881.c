/*
 * A221881  Number of order-preserving or order-reversing full contraction
 *          mappings (of an n-chain) with (right) waist exactly k.
 * https://oeis.org/A221881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 2*Sum_{p=1..k} C(n-1,p-1) - 1 (for k >= 1) The sequence
 *            a(N) is the flattened triangle of T(n,k) in row-major order:
 *            a(1) = T(1,1), a(2) = T(2,1), a(3) = T(2,2), a(4) = T(3,1), …
 * Range:     u8 n <= 40, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* idx_t is an unsigned integer type; seq_t is the output type (uintX_t or arbitrary-precision) */
seq_t a(idx_t N)
{
    if (N == 0)          /* not part of OEIS offset but defensive */
        return (seq_t)0;

    /* Find the row r such that S(r-1) < N <= S(r) where S(r)=r*(r+1)/2 */
    idx_t r = 1;
    idx_t cum = 0;              /* cumulative triangular sum */
    while ((cum += r) < N)
        ++r;
    /* Now cum = S(r) and the previous cumulative sum is S(r-1) = cum - r */
    idx_t tri_prev = cum - r;   /* S(r-1) */
    idx_t k = N - tri_prev;     /* column index, 1 <= k <= r */

    seq_t sum = 0;
    /* Sum over i = 0 .. k-1 of binomial(r-1, i) */
    for (idx_t i = 0; i < k; ++i)
        sum += binom(r - 1, i);

    /* Compute T(r,k) = 2*sum - 1 */
    return sum * (seq_t)2 - (seq_t)1;
}
