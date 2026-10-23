/*
 * A161862  Number of reduced words of length n in the Weyl group B_14.
 * https://oeis.org/A161862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the coefficient of xⁿ in ∏_{k=1..14} (1‑x^(2k))/(1‑x)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 26, u64 n <= 196, u128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It equals the number of solutions to ∑_{k=1}^{14} a_k = n with 0≤a_k≤2k‑1.  */
#include <stdbool.h>

#define MAX_N 196   /* longest reduced word length in B₁₄ */

/* Computes A161862(n).  For n>MAX_N the value is zero. */
seq_t a(idx_t n)
{
    static seq_t dp[MAX_N + 1];
    static bool ready = false;

    if (!ready) {
        /* initialise dp[0] = 1, all others 0 */
        for (idx_t i = 0; i <= MAX_N; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* iterate over each factor (1 + x + … + x^{2k‑1}) */
        for (idx_t k = 1; k <= 14; ++k) {
            idx_t limit = 2 * k - 1;
            seq_t newdp[MAX_N + 1];

            /* reset newdp */
            for (idx_t i = 0; i <= MAX_N; ++i)
                newdp[i] = 0;

            for (idx_t sum = 0; sum <= MAX_N; ++sum) {
                seq_t val = dp[sum];
                if (!val)
                    continue;
                /* add val to all positions sum + t where 0≤t≤limit */
                for (idx_t t = 0; t <= limit && sum + t <= MAX_N; ++t)
                    newdp[sum + t] += val;
            }

            /* copy back to dp for next factor */
            for (idx_t i = 0; i <= MAX_N; ++i)
                dp[i] = newdp[i];
        }

        ready = true;
    }

    if (n > MAX_N)
        return 0;
    return dp[n];
}
