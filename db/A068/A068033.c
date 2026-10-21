/*
 * A068033  Number of subsets of {1,2,3,...,n} that sum to 0 mod 12.
 * https://oeis.org/A068033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP over residues mod 12. */
/* Computes a(n) = number of subsets of {1,2,…,n} with sum ≡ 0 (mod 12). */

seq_t a(idx_t n)
{
    seq_t dp[12];
    seq_t tmp[12];
    int i;

    /* initialize dp[0] = 1, others 0 */
    for (i = 0; i < 12; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    if (n == 0) return dp[0];

    for (idx_t num = 1; num <= n; ++num) {
        /* copy current dp to tmp */
        for (i = 0; i < 12; ++i) tmp[i] = dp[i];

        int shift = (int)(num % 12);
        /* update dp by including number 'num' */
        for (i = 0; i < 12; ++i) {
            int r2 = i + shift;
            if (r2 >= 12) r2 -= 12;
            dp[r2] += tmp[i];
        }
    }

    return dp[0];
}
