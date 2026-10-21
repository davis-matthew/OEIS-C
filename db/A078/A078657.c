/*
 * A078657  Expansion of 1/Product_{ n >= 2, n not of the form 2^k-1 } (1 -
 *          x^n).
 * https://oeis.org/A078657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n in which no part is of the form
 *            2^k – 1 (k ≥ 1). Computed by the standard coin‑change DP
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_forbidden
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_forbidden.h"

seq_t a(idx_t n)
{
    /* Allocate an array of dp[0..n], all zero. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1U) * sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;        /* allocation failure – unlikely */

    /* initialise to zero; dp[0] will be set to 1 */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;

    dp[0] = (seq_t)1;          /* there is exactly one way to partition 0 */

    /* Process each admissible part one by one. */
    for (idx_t part = 1; part <= n; ++part) {
        if (is_forbidden(part))
            continue;          /* skip forbidden parts */

        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] = dp[sum] + dp[sum - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
