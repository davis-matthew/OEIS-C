/*
 * A022236  Gaussian binomial coefficients [ n,7 ] for q = 7.
 * https://oeis.org/A022236
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Gaussian binomial coefficient [n,7]_7 = Product_{i=1..7}
 *            (7^(n-i+1)-1)/(7^i-1)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 7) {
        /* The sequence is defined for n >= 7; other values are undefined. */
        return 0;
    }

    /* dp[k] holds [current i, k]_7 for k = 0..7; dp[0] is always 1. */
    seq_t dp[8] = {0};
    dp[0] = (seq_t)1;

    /* pow_start = 7^{i - maxk}. For i <= 7, pow_start = 1. */
    seq_t pow_start = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        /* Determine max k for this row. */
        int maxk = (i < 7) ? (int)i : 7;

        /* Update pow_start each time i grows beyond 7. */
        if (i >= 8) {
            pow_start = pow_start * (seq_t)7;
        }

        /* pow_factor will iterate from 7^{i-maxk} up to 7^1 as we
           descend in k. */
        seq_t pow_factor = pow_start;

        for (int k = maxk; k >= 1; --k) {
            /* dp[k] = dp[k] + 7^{i-k} * dp[k-1] */
            seq_t term = pow_factor * dp[k - 1];
            dp[k] = dp[k] + term;
            /* Advance pow_factor to 7^{i-(k-1)} for the next iteration. */
            pow_factor = pow_factor * (seq_t)7;
        }
    }

    return dp[7];
}
