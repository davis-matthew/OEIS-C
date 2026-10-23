/*
 * A151692  G.f.: Product_{k>=2} (1 + x^(2^k-1) + x^(2^k)).
 * https://oeis.org/A151692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as a sum of distinct terms from
 *            the set { 2^k−1, 2^k : k ≥ 2 }, using at most one of the two
 *            terms for each k. In other words a(n) = coeff of x^n in ∏_{k≥2}
 *            (1 + x^(2^k−1) + x^(2^k))
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate two DP arrays of size n+1, zeroed.  */
    size_t sz = (size_t)(n + 1);
    seq_t *dp     = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *newdp  = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp || !newdp) {             /* out of memory – return 0 */
        free(dp); free(newdp);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;                 /* empty sum */

    for (idx_t k = 2; ; ++k) {
        idx_t v1 = ((idx_t)1 << k) - 1;   /* 2^k − 1 */
        idx_t v2 = ((idx_t)1 << k);       /* 2^k */

        if (v1 > n && v2 > n)
            break;                       /* no further contributions */

        /* Start with the old counts */
        for (size_t i = 0; i <= n; ++i)
            newdp[i] = dp[i];

        /* Add ways using weight 2^k−1 */
        if (v1 <= n) {
            for (size_t i = (size_t)v1; i <= n; ++i)
                newdp[i] += dp[i - (size_t)v1];
        }

        /* Add ways using weight 2^k */
        if (v2 <= n) {
            for (size_t i = (size_t)v2; i <= n; ++i)
                newdp[i] += dp[i - (size_t)v2];
        }

        /* Prepare for next iteration */
        seq_t *tmp = dp;
        dp        = newdp;
        newdp     = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(newdp);
    return result;
}
