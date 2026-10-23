/*
 * A022240  Gaussian binomial coefficients [ n,11 ] for q = 7.
 * https://oeis.org/A022240
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..11} (7^(n-i+1)-1)/(7^i-1)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 12, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via recurrence of Gaussian binomial coefficients:
   [n,k]_q = [n-1,k]_q + q^{n-k} [n-1,k-1]_q  with q=7, k=11 */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n < 11) return 0;               /* values below the offset are undefined */

    const int K = 11;                    /* fixed upper index k */
    const seq_t Q = (seq_t)7;            /* field size q */

    /* Pre‑compute powers Q^i for i = 0 .. n  */
    seq_t *powq = alloc_seq_array((size_t)n + 1);
    powq[0] = (seq_t)1;
    for (size_t i = 1; i <= (size_t)n; ++i)
        powq[i] = powq[i-1] * Q;

    /* DP arrays: dp[j] holds [current_n, j]_Q  */
    seq_t dp[K+1], next[K+1];
    for (int j = 0; j <= K; ++j) dp[j] = 0;
    dp[0] = (seq_t)1;                    /* [any,0] = 1 */

    for (size_t i = 1; i <= (size_t)n; ++i) {
        int maxj = (i < (size_t)K) ? (int)i : K;
        next[0] = (seq_t)1;               /* [i,0] = 1 */
        for (int j = 1; j <= maxj; ++j) {
            /* term = Q^{i-j} * dp[j-1] */
            seq_t term = dp[j-1];
            term = term * powq[i-j];
            next[j] = dp[j] + term;       /* [i,j] = [i-1,j] + Q^{i-j}[i-1,j-1] */
        }
        for (int j = maxj+1; j <= K; ++j) next[j] = 0;
        /* swap dp and next */
        for (int j = 0; j <= K; ++j) dp[j] = next[j];
    }

    free_seq_array(powq);
    return dp[K];
}
