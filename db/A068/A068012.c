/*
 * A068012  Number of subsets of {1,2,3,...,n} that sum to 0 mod 6.
 * https://oeis.org/A068012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,n} whose sum is ≡0 (mod 6).
 *            Computation: dynamic programming over residues modulo 6
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* dp[r] = number of subsets of the first processed elements whose sum ≡ r (mod 6) */
    seq_t dp[6] = {1, 0, 0, 0, 0, 0};
    for (idx_t k = 1; k <= n; ++k) {
        idx_t mod = k % 6;
        seq_t newdp[6];
        for (idx_t r = 0; r < 6; ++r) {
            /* element k is excluded: dp[r] */
            /* element k is included: dp[(r - mod) mod 6] */
            seq_t without = dp[r];
            seq_t with    = dp[(r + 6 - mod) % 6];
            newdp[r] = without + with;
        }
        for (idx_t r = 0; r < 6; ++r) dp[r] = newdp[r];
    }
    return dp[0];   /* subsets with sum ≡ 0 (mod 6) */
}
