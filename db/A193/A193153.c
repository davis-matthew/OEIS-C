/*
 * A193153  Numbers of spanning trees in the graph join of C_n and C_n.
 * https://oeis.org/A193153
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( ( 2*(T_n((n+2)/2)-1) / n ) )^2 where T_n is the
 *            Chebyshev polynomial of first kind. The derivation uses the
 *            eigenvalues of the Laplacian of the join of two n‑cycles. The
 *            recurrence below computes the required Chebyshev value exactly
 *            using only integer arithmetic
 * Range:     u8 none, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       /* offset starts at 3 */

    /* m = n + 2 (as seq_t) */
    seq_t m = (seq_t)n + 2;
    /* constants in the recurrence */
    seq_t two_m   = m + m;               /* 2*m   */
    seq_t four    = (seq_t)4;            /* 4     */

    /* S_0 = 1 , S_1 = m  */
    seq_t S_prev2 = (seq_t)1;            /* S_{k-2} */
    seq_t S_prev1 = m;                   /* S_{k-1} */
    seq_t S_curr  = S_prev1;             /* will hold S_k */

    /* Compute S_k for k = 2 .. n   (S_n is needed) */
    for (idx_t k = 2; k <= n; ++k)
    {
        S_curr = two_m * S_prev1 - four * S_prev2;
        S_prev2 = S_prev1;
        S_prev1 = S_curr;
    }

    /* S_n is now in S_curr */
    seq_t two_pow_n          = pow2(n);          /* 2^n   */
    seq_t two_pow_n_minus_1  = pow2(n - 1);      /* 2^(n-1) */

    /* Q = (S_n - 2^n) / ( n * 2^(n-1) ) */
    seq_t numerator   = S_curr - two_pow_n;
    seq_t denominator = ((seq_t)n) * two_pow_n_minus_1;
    seq_t Q = numerator / denominator;          /* exact division */

    return Q * Q;
}
