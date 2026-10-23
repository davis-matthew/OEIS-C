/*
 * A161526  Number of reduced words of length n in the Weyl group A_26.
 * https://oeis.org/A161526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..26} (1 + x + … + x^k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 51, u128 n <= 351,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    const idx_t MAXN = 351;                       /* 1+2+…+26 */
    static seq_t dp[352];
    static seq_t newdp[352];
    static bool init = false;                       /* one‑time initialization */

    if (!init) {
        /* initialise dp[0] = 1, others 0 */
        for (idx_t i = 0; i <= MAXN; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* iterate over k = 1 .. 26, convolve with (1 + x + … + x^k) */
        for (idx_t k = 1; k <= 26; ++k) {
            for (idx_t i = 0; i <= MAXN; ++i) newdp[i] = (seq_t)0;   /* clear newdp */

            for (idx_t i = 0; i <= MAXN; ++i) {
                for (idx_t t = 0; t <= k && t <= i; ++t) {
                    newdp[i] = newdp[i] + dp[i - t];
                }
            }

            /* copy newdp back to dp for the next iteration */
            for (idx_t i = 0; i <= MAXN; ++i) dp[i] = newdp[i];
        }
        init = true;
    }

    if (n > MAXN) return (seq_t)0;
    return dp[n];
}
