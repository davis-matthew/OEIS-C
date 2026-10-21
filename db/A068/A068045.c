/*
 * A068045  Number of subsets of {1,2,3,...,n} that sum to 0 mod 64.
 * https://oeis.org/A068045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of subsets of {1,2,…,n} whose sum is 0 modulo 64.
 *            Computed by dynamic programming over the modulus 64
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 37, u64 n <= 69, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Local arrays to hold counts for each remainder modulo 64. */
    seq_t dp[64];
    seq_t dp_next[64];

    /* Initialise: only the empty set is counted for sum 0. */
    for (int i = 0; i < 64; i++)
        dp[i] = 0;
    dp[0] = 1;

    /* Iterate over each element 1..n, updating the counts. */
    for (idx_t x = 1; x <= n; ++x) {
        int shift = (int)(x & 63);            /* x mod 64 */
        for (int r = 0; r < 64; r++) {
            int prev = (r + 64 - shift) & 63; /* (r - shift) mod 64 */
            dp_next[r] = dp[r] + dp[prev];
        }
        /* Transfer next state into current state. */
        for (int r = 0; r < 64; r++)
            dp[r] = dp_next[r];
    }

    return dp[0];   /* Count of subsets with sum ≡ 0 (mod 64). */
}
