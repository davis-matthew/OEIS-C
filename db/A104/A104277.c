/*
 * A104277  Number of partitions of n in which both even and odd squares occur
 *          with multiplicity 1. There is no restriction on the parts which
 *          are twice odd squares.
 * https://oeis.org/A104277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   G.f. = Π_{k>0} (1 + x^{(2k)^2}) / (1 - x^{(2k-1)^2})
 * Range:     u8 n <= 106, u16 n <= 411, u32 n <= 1923, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (even squares occur at most once; odd squares unlimited) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate and zero the DP array */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure – unlikely in tests */

    seq_t one = 1;
    dp[0] = one;                /* base case: one way to form 0 */

    for (idx_t k = 1; ; ++k)
    {
        idx_t sq = k * k;       /* square value */
        if (sq > n) break;

        if ((k & 1) == 0)      /* even k → even square: at most one */
        {
            for (idx_t j = n; j >= sq; --j)
                dp[j] += dp[j - sq];
        }
        else                    /* odd k → odd square: unlimited */
        {
            for (idx_t j = sq; j <= n; ++j)
                dp[j] += dp[j - sq];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
