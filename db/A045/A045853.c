/*
 * A045853  Number of nonnegative solutions of x1^2 + x2^2 + ... + x12^2 = n.
 * https://oeis.org/A045853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of nonnegative solutions of x1^2 + x2^2 + ... + x12^2 = n
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 244, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑compute all square numbers ≤ n. */
    idx_t i;
    idx_t sq_cnt = 0;
    for (i = 0; i * i <= n; i++)
        sq_cnt++;

    idx_t *sqs = (idx_t *)malloc(sq_cnt * sizeof(idx_t));
    if (!sqs) return 0; /* allocation failed – unlikely in contest environment */
    for (i = 0; i * i <= n; i++)
        sqs[i] = i * i;

    /* Allocate two DP arrays and initialise. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *next = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp || !next) {
        free(sqs);
        return 0;
    }

    dp[0] = 1;

    /* Iterate over the 12 variables. */
    for (i = 0; i < 12; i++) {
        /* Reset next array to zero. */
        for (idx_t k = 0; k <= n; k++)
            next[k] = 0;

        for (idx_t sum = 0; sum <= n; sum++) {
            if (dp[sum] == 0)
                continue;

            for (idx_t s = 0; s < sq_cnt; s++) {
                idx_t t = sqs[s];
                if (sum + t > n)
                    break;
                next[sum + t] += dp[sum];
            }
        }

        /* Swap dp and next for next iteration. */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[n];

    free(dp);
    free(next);
    free(sqs);

    return result;
}
