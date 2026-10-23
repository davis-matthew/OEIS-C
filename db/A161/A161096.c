/*
 * A161096  Number of partitions of n into squares where every part appears at
 *          least 8 times.
 * https://oeis.org/A161096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the coefficient of x^n in ∏_{k≥1} (1 +
 *            x^(8*k^2)/(1-x^(k^2))) (OEIS A161096)
 * Range:     u8 n <= 211, u16 n <= 660, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index 1 start; a(1)=0 */

    /* Allocate arrays for DP */
#ifdef __cplusplus
    seq_t *dp   = new seq_t[n + 1]();   /* value‑initialized to 0 */
    seq_t *tmp  = new seq_t[n + 1]();
#else
    seq_t *dp   = calloc(n + 1, sizeof(seq_t));   /* zeroed */
    seq_t *tmp  = calloc(n + 1, sizeof(seq_t));
#endif

    dp[0] = 1;                       /* empty partition of 0 */

    /* Process each square weight w <= n */
    for (idx_t k = 1;; ++k) {
        idx_t w = k * k;
        if (w > n) break;

        /* Copy old DP to tmp */
        for (idx_t j = 0; j <= n; ++j) tmp[j] = dp[j];

        /* Add contributions of this weight */
        for (idx_t r = 0; r < w; ++r) {
            seq_t run = 0;          /* accumulates dp[j-8*w] for j = r + m*w  */
            for (idx_t m = 0;; ++m) {
                idx_t j = r + m * w;
                if (j > n) break;

                if (m >= 8) {
                    idx_t sub = j - 8 * w;
                    run += dp[sub];
                }
                tmp[j] += run;
            }
        }

        /* Prepare for next weight */
        seq_t *swap = dp;
        dp = tmp;
        tmp = swap;
    }

    seq_t result = dp[n];

    /* Free memory */
#ifdef __cplusplus
    delete[] dp;
    delete[] tmp;
#else
    free(dp);
    free(tmp);
#endif

    return result;
}
