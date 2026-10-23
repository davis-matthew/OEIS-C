/*
 * A161679  Number of reduced words of length n in the Weyl group A_44.
 * https://oeis.org/A161679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Prod_{k=1..44} (1 + x + ... + x^k).
 *            This product equals the generating function for reduced word
 *            lengths in the Weyl group A_44. The maximum length is 44*45/2 =
 *            990, so a(n)=0 for n>990. The function precomputes all
 *            coefficients at first call and returns a(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool initialized = false;
#define MAX_N ((44*45)/2)          /* 990 */

/* DP array for coefficients of the generating function. */
static seq_t dp[MAX_N+1];

/* Return a(n).  n is unsigned idx_t. */
seq_t a(idx_t n)
{
    /* Precompute once. */
    if (!initialized) {
        /* Clear the array. */
        for (int i = 0; i <= MAX_N; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* Temporary array for convolution. */
        static seq_t tmp[MAX_N+1];

        for (int k = 1; k <= 44; ++k) {
            /* tmp = 0 */
            for (int i = 0; i <= MAX_N; ++i)
                tmp[i] = 0;

            /* Convolve dp with 1 + x + ... + x^k */
            for (int i = 0; i <= MAX_N; ++i) {
                /* Skip zero terms to reduce work. */
                if (dp[i] == 0) continue;
                /* Add dp[i] to tmp[i + t] for t = 0..k (bounded by MAX_N). */
                for (int t = 0; t <= k && i + t <= MAX_N; ++t)
                    tmp[i + t] += dp[i];
            }

            /* Copy back to dp. */
            for (int i = 0; i <= MAX_N; ++i)
                dp[i] = tmp[i];
        }

        initialized = true;
    }

    if (n > (idx_t)MAX_N)
        return 0;

    return dp[(size_t)n];
}
