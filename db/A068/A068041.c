/*
 * A068041  Number of subsets of {1,2,3,...,n} that sum to 0 mod 20.
 * https://oeis.org/A068041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,n} whose sum ≡ 0 (mod 20).
 *            Computed via the recurrence dp[0] = 1, dp[k] = 0 (k = 1…19) at n
 *            = 0 for i = 1 … n: shift = i mod 20 for m = 0 … 19: next[m] =
 *            dp[m] + dp[(m - shift + 20) mod 20] a(n) = dp[0] after the loop
 *            The recurrence counts, for each n, how many...
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 20-state DP for residues modulo 20. */
    seq_t cur[20] = {0};
    seq_t next[20] = {0};

    cur[0] = (seq_t)1;          /* empty set for n = 0 */

    for (idx_t i = 1; i <= n; ++i) {
        unsigned int shift = (unsigned int)(i % 20);

        for (int m = 0; m < 20; ++m) {
            int src = m - shift;
            if (src < 0) src += 20;
            next[m] = cur[m] + cur[src];
        }

        /* copy next into cur for the next iteration */
        for (int m = 0; m < 20; ++m) cur[m] = next[m];
    }

    return cur[0];
}
