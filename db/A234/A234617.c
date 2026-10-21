/*
 * A234617  Numbers of undirected cycles in the 2n-crossed prism graph.
 * https://oeis.org/A234617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) + 4^n + (2^(n+1) - 3) * n (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* 2^(n‑1) */
    seq_t term1 = pow2((idx_t)(n - 1));

    /* 4^n */
    seq_t term2 = pow4((idx_t)n);

    /* (2^(n+1) - 3) * n */
    seq_t pow2_plus1   = pow2((idx_t)(n + 1));
    seq_t term3 = (pow2_plus1 - (seq_t)3) * (seq_t)n;

    return term1 + term2 + term3;
}
