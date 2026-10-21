/*
 * A205217  Number of partitions of n into distinct parts having no 2 in
 *          ternary representation.
 * https://oeis.org/A205217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 370, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   has_no_ternary_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205217:
 * a(n) = number of partitions of n into distinct parts,
 * each part having no digit 2 in its base‑3 representation
 * (i.e., every allowed part is a sum of distinct powers of 3). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_no_ternary_two.h"

seq_t a(idx_t n)
{
    /* DP array for counting subset sums of allowed parts */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failure */

    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    for (idx_t m = 1; m <= n; ++m) {          /* iterate possible parts */
        if (!has_no_ternary_two(m)) continue;

        /* Add this part to all reachable sums, going downward to
           guarantee each part is used at most once. */
        for (idx_t i = n; i >= m; --i) {
            dp[i] += dp[i - m];
            if (i == m) break;              /* avoid unsigned wrap‑around */
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
