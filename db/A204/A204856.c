/*
 * A204856  G.f.: Sum_{n>=0} x^(n*(n+1)/2) / Product_{k=1..n} (1 - k*x^k).
 * https://oeis.org/A204856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 55, u64 n <= 114,
 *            u128 n <= 233, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A204856: 
 * a(n) is the coefficient of xⁿ in 
 *   Σ_{m≥0}  x^{m(m+1)/2} / Π_{k=1..m} (1 - k·xᵏ)
 * The algorithm builds the partial products Π_{k=1..m} (1 - k·xᵏ)⁻¹
 * incrementally and accumulates contributions using a standard
 * unbounded‑knapsack DP with a fast in‑place recurrence.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate coefficient arrays a[0..n] and dp[0..n] */
    seq_t *a_arr = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp    = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!a_arr || !dp) {
        free(a_arr);
        free(dp);
        return (seq_t)0;          /* allocation failed – undefined */
    }

    /* m = 0 : offset = 0, product = 1 */
    a_arr[0] = (seq_t)1;
    dp[0]    = (seq_t)1;

    /* iterate over m = 1, 2, ... while the offset fits in n */
    for (idx_t m = 1; ; ++m) {
        seq_t offset = ((seq_t)m * ((seq_t)m + (seq_t)1)) / (seq_t)2;
        if (offset > n) break;                 /* no further contributions */

        /* --- update dp to multiply by 1/(1 - m·xᵐ) --- */
        for (idx_t r = 0; r < m; ++r) {
            seq_t accum = (seq_t)0;
            idx_t maxS  = (n - r) / m;
            for (idx_t s = 0; s <= maxS; ++s) {
                idx_t idx  = r + s * m;
                seq_t old   = dp[idx];
                accum = accum * (seq_t)m + old;
                dp[idx] = accum;               /* in‑place update */
            }
        }

        /* --- add contributions for this m --- */
        for (idx_t N = (idx_t)offset; N <= n; ++N) {
            a_arr[N] += dp[N - (idx_t)offset];
        }
    }

    seq_t result = a_arr[n];
    free(dp);
    free(a_arr);
    return result;
}
