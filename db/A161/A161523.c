/*
 * A161523  Number of reduced words of length n in the Weyl group A_23.
 * https://oeis.org/A161523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) equals the coefficient of x^n in ∏_{k=1}^{23} (1 + x + … +
 *            x^k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 67, u128 n <= 276,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_N 276   /* 1+2+…+23 */

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t cache[MAX_N + 1];

    if (!ready) {
        seq_t dp[MAX_N + 1];
        seq_t newdp[MAX_N + 1];

        /* initialise dp to zero */
        for (int i = 0; i <= MAX_N; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;     /* start polynomial 1 */

        for (int k = 1; k <= 23; ++k) {
            /* zero newdp */
            for (int i = 0; i <= MAX_N; ++i) newdp[i] = (seq_t)0;

            /* multiply current polynomial by (1 + x + … + x^k) */
            for (int i = 0; i <= MAX_N; ++i) {
                seq_t val = dp[i];
                if (val == (seq_t)0) continue;
                for (int j = 0; j <= k; ++j) {
                    int idx = i + j;
                    if (idx > MAX_N) break;
                    newdp[idx] += val;
                }
            }

            /* copy newdp back into dp for next iteration */
            for (int i = 0; i <= MAX_N; ++i) dp[i] = newdp[i];
        }

        /* store the coefficients for future calls */
        for (int i = 0; i <= MAX_N; ++i) cache[i] = dp[i];
        ready = true;
    }

    if (n <= (idx_t)MAX_N) {
        return cache[(size_t)n];
    } else {
        return (seq_t)0;
    }
}
