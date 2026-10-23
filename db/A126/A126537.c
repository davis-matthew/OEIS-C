/*
 * A126537  Number of base 16 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Number of base 16 n-digit numbers with adjacent digits differing
 *            by five or less
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    seq_t dp[16], ndp[16];
    /* Initialise count for length 1: all digits allowed */
    for (int j = 0; j < 16; ++j) dp[j] = (seq_t)1;

    /* Extend sequences up to length n */
    for (idx_t len = (idx_t)1; len < n; ++len) {
        for (int j = 0; j < 16; ++j) {
            seq_t sum = (seq_t)0;
            int start = j - 5;
            if (start < 0) start = 0;
            int end = j + 5;
            if (end > 15) end = 15;
            for (int k = start; k <= end; ++k) {
                sum += dp[k];
            }
            ndp[j] = sum;
        }
        /* Copy new counts to dp for next iteration */
        for (int j = 0; j < 16; ++j) dp[j] = ndp[j];
    }

    /* Total number of n‑digit sequences */
    seq_t result = (seq_t)0;
    for (int j = 0; j < 16; ++j) result += dp[j];
    return result;
}
