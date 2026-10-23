/*
 * A100684  Number of partitions of 2n free of multiples of 8 such that 4
 *          occurs at most once. All odd parts occur with even multiplicities.
 *          There is no restriction on the other even parts.
 * https://oeis.org/A100684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of 2n with no parts that are
 *            multiples of 8, at most one part equal to 4, and every odd part
 *            appearing with even multiplicity. Generating function:
 *            (1-x^4)*∏_{i≥1}(1+x^{2i})/(1-x^{2i-1})^2
 * Range:     u8 n <= 11, u16 n <= 32, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t M = 2 * n;                      /* target sum 2n   */
    size_t sz = (size_t)(M + 1);          /* array size      */
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return 0;                    /* allocation fail */

    /* initialise dp[0] = 1, others 0 */
    for (size_t i = 0; i < sz; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;

    for (idx_t j = 1; j <= M; ++j) {
        if (j & 1) {                      /* odd part          */
            idx_t step = 2 * j;
            for (idx_t s = step; s <= M; ++s)
                dp[s] += dp[s - step];
        } else {                          /* even part         */
            if (j % 8 == 0) continue;     /* forbidden multiples of 8 */
            if (j == 4) {                 /* at most one part 4 */
                for (idx_t s = M; s >= 4; --s)
                    dp[s] += dp[s - 4];
            } else {                      /* unlimited even part */
                for (idx_t s = j; s <= M; ++s)
                    dp[s] += dp[s - j];
            }
        }
    }

    seq_t res = dp[M];
    free(dp);
    return res;
}
