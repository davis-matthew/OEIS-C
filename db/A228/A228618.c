/*
 * A228618  Sum of lengths of shortest runs with the same value over all s in
 *          {1,...,n}^n.
 * https://oeis.org/A228618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} G_k G_k = sum_{r=1}^{floor(n/k)} n *
 *            (n-1)^{r-1} * C( n - (k-1)r - 1 , r-1 ) where C(N,K) is the
 *            binomial coefficient
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: binomial coefficient C(N,K) for 0 ≤ K ≤ N.
   Uses multiplicative formula: C(N,K) = Π_{i=1..K} (N-K+i)/i
   Exact division is guaranteed.
*/
static seq_t binom(idx_t N, idx_t K) {
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (seq_t)(N - K + i);   // multiply by integer
        res /= (seq_t)i;                  // exact division
    }
    return res;
}

/* Main function to compute a(n) for any n ≥ 0. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */

    seq_t total = 0;
    seq_t n_seq = (seq_t)n;
    seq_t nm1_seq = (seq_t)(n - 1);
    seq_t pow_val = 1;                    /* (n-1)^(r-1), starts with r=1 */

    for (idx_t r = 1; r <= n; ++r) {
        /* number of possible runs (r) and how many times we shift the block size */
        idx_t m = n / r;                  /* floor(n/r) */

        /* Sum over positions of the minimal runs: Σ_{t=0}^{m-1} C(n - t*r - 1, r-1) */
        seq_t sum_j = 0;
        for (idx_t t = 0; t < m; ++t) {
            idx_t N = n - t * r - 1;      /* the upper parameter of binomial */
            seq_t bin = binom(N, r - 1);
            sum_j += bin;
        }

        /* Contribution to a(n) from this r */
        seq_t contrib = n_seq * pow_val * sum_j;
        total += contrib;

        /* Prepare pow_val for next r (r increases by 1) */
        if (r < n) {
            pow_val *= nm1_seq;
        }
    }

    return total;
}
