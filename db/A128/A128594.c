/*
 * A128594  Column 2 of triangle A128592; a(n) = coefficient of q^(2n+6) in
 *          the q-analog of the odd double factorials (2n+5)!! for n>=0.
 * https://oeis.org/A128594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^(2n+6) in ∏_{j=1..n+3}
 *            (1-q^(2j-1))/(1-q)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This is computed as the number of ways to write 2n+6 as a sum of n+3
 *  bounded non‑negative integers k_j (0 ≤ k_j ≤ 2j-2).  The DP below
 *  updates the polynomial coefficients efficiently using prefix sums.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
    #include <vector>
    #include <utility>
#endif

seq_t a(idx_t n)
{
    /* N = n+3, target exponent e = 2n+6 = 2N */
    idx_t N = n + 3;
    idx_t e = 2 * N;          /* exponent to extract */
    idx_t sz = e + 1;         /* number of coefficient slots 0..e */

    /* allocation of coefficient arrays */
#ifdef __cplusplus
    std::vector<seq_t> dp(sz), pref(sz), newdp(sz);
    dp[0] = 1;
#else
    seq_t *dp    = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *pref  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *newdp = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp || !pref || !newdp) {
        /* allocation failure – return 0 as safe fallback */
        free(dp); free(pref); free(newdp);
        return (seq_t)0;
    }
    dp[0] = 1;
#endif

    /* iterate over each factor j = 1 .. N  (length 2j-1) */
    for (idx_t j = 1; j <= N; ++j) {

        idx_t limit = 2 * j - 2;   /* maximal exponent contributed by this factor */

        /* prefix sums of current dp */
#ifdef __cplusplus
        pref[0] = dp[0];
        for (idx_t i = 1; i <= e; ++i) pref[i] = pref[i-1] + dp[i];
#else
        pref[0] = dp[0];
        for (idx_t i = 1; i <= e; ++i) pref[i] = pref[i-1] + dp[i];
#endif

        /* compute newdp using sliding window over prefix sums */
        for (idx_t k = 0; k <= e; ++k) {
            if (k <= limit) {
#ifdef __cplusplus
                newdp[k] = pref[k];
#else
                newdp[k] = pref[k];
#endif
            } else {
#ifdef __cplusplus
                newdp[k] = pref[k] - pref[k - limit - 1];
#else
                newdp[k] = pref[k] - pref[k - limit - 1];
#endif
            }
        }

        /* swap dp and newdp for next iteration */
#ifdef __cplusplus
        std::swap(dp, newdp);
#else
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
#endif
    }

    seq_t result = dp[e];

    /* clean‑up for C build */
#ifndef __cplusplus
    free(dp);
    free(pref);
    free(newdp);
#endif

    return result;
}
