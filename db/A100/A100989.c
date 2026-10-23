/*
 * A100989  Number of partitions of n into parts free of odd hexagonal numbers
 *          and the only number with multiplicity in the unrestricted
 *          partitions is the number 2 with multiplicity of the form 3k+l,
 *          where k is a positive integer and l=0,1.
 * https://oeis.org/A100989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* DP coefficients for the product GF */
    idx_t size = n + 1;
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) return 0;                 /* fallback on allocation failure */

    dp[0] = 1;
    for (idx_t i = 1; i < size; ++i) dp[i] = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t m = 2 * k * k - k;         /* exponent of the denominator term */
        int r = (k & 1) ? -1 : 1;       /* r = (-1)^k */

        /* Multiply by 1 / (1 - r*x^m) */
        if (m <= n) {
            for (idx_t i = m; i <= n; ++i) {
                if (r == 1)
                    dp[i] += dp[i - m];
                else
                    dp[i] -= dp[i - m];   /* unsigned subtraction wraps modulo 2^w */
            }
        }

        /* Multiply by (1 + x^k) */
        for (idx_t i = n; i >= k; --i) {
            dp[i] += dp[i - k];
        }
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
