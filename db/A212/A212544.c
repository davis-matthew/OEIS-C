/*
 * A212544  Number of partitions of n containing at least one part m-4 if m is
 *          the largest part.
 * https://oeis.org/A212544
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=5}^{⌊(n+4)/2⌋} p(n-2m+4, m) where p(k, m) is the
 *            number of partitions of k with parts ≤ m
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 147, u64 n <= 461,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 4) return 0;

    /* Allocate DP array for partitions with parts ≤ current m. */
    idx_t N = n + 1;
    seq_t *dp = (seq_t *)malloc((size_t)N * sizeof(seq_t));
    if (!dp) return 0;                     /* out‑of‑memory guard */
    for (size_t i = 0; i < (size_t)N; ++i) dp[i] = 0;

    dp[0] = 1;         /* empty partition */
    seq_t result = 0;

    idx_t mmax = (n + 4) / 2;  /* largest m that can contribute */
    for (idx_t m = 1; m <= mmax; ++m) {
        /* Update DP to include part size m (unbounded number of copies). */
        for (idx_t k = m; k <= n; ++k) {
            dp[(size_t)k] += dp[(size_t)(k - m)];
        }

        /* If m is a valid largest part, add contribution. */
        if (m >= 5) {
            idx_t k_val = n - 2 * m + 4;   /* remaining sum after taking one m and one m-4 */
            if (k_val <= n) {              /* k_val ≥ 0 automatically for m ≤ mmax */
                result += dp[(size_t)k_val];
            }
        }
    }

    free(dp);
    return result;
}
