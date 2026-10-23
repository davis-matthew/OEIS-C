/*
 * A104501  Coefficients of the A-Dyson Mod 27 identity.
 * https://oeis.org/A104501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the number of partitions of n into parts not
 *            divisible by 27 and not congruent to 12 or 15 modulo 27. This
 *            follows from the generating function ∏_{k>0}
 *            (1-x^(27k))(1-x^(27k-12))(1-x^(27k-15))/(1-x^k) which simplifies
 *            to 1 / ∏_{k≥1, k%27∉{0,12,15}} (1-x^k)
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 134, u64 n <= 446,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* allocate array of size n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp) return 0;            /* allocation failure; unlikely for test sizes */

    dp[0] = 1;                    /* empty partition */

    /* iterate over all allowed part sizes k */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t mod = k % 27;
        if (mod == 0 || mod == 12 || mod == 15)
            continue;            /* forbidden residue */

        /* classic partition DP for unlimited copies of part k */
        for (idx_t i = k; i <= n; ++i) {
            dp[i] += dp[i - k];
        }
    }

    seq_t ans = dp[n];
    free(dp);
    return ans;
}
