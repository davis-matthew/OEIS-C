/*
 * A026829  Number of partitions of n into distinct parts, the least being 8.
 * https://oeis.org/A026829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts * with the
 *            least part equal to 8. For n < 8 the value is 0; * for n ≥ 8,
 *            let m = n – 8 and compute the number of partitions * of m into
 *            distinct parts with all parts ≥ 9
 * Range:     u8 n <= 70, u16 n <= 139, u32 n <= 338, u64 n <= 969,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;

    /* m is the offset: n = m + 8.
     * We need partitions of m into distinct parts ≥ 9. */
    idx_t m = n - 8;

    /* Static buffer reused between calls.  dp[s] holds the
     * number of partitions of s using distinct parts ≥ 9. */
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;

    /* Grow the buffer if necessary. */
    if (dp == NULL || dp_cap < m + 1) {
        dp_cap = m + 1;
        dp = (seq_t *)realloc(dp, dp_cap * sizeof(seq_t));
        if (dp == NULL)
            return 0; /* allocation failure – unlikely in tests */
    }

    /* Initialise dp[0] = 1 and all other dp[1..m] = 0. */
    dp[0] = 1;
    for (idx_t i = 1; i <= m; ++i)
        dp[i] = 0;

    /* Classic subset‑sum DP: for each part k ≥ 9, add it to every
     * attainable sum in reverse order to avoid reuse of the same
     * part more than once. */
    for (idx_t part = 9; part <= m; ++part) {
        for (idx_t sum = m; sum >= part; --sum) {
            dp[sum] += dp[sum - part];
        }
    }

    return dp[m];
}
