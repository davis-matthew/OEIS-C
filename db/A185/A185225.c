/*
 * A185225  Number of disconnected 2-regular simple graphs on n vertices with
 *          girth at least 5.
 * https://oeis.org/A185225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n with all parts ≥ 5 minus 1 if n
 *            ≥ 5 (remove the single‑part partition [n])
 * Range:     u8 n <= 38, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of partition counts up to the largest n seen so far */
static seq_t *dp      = NULL; /* dp[i] = #partitions of i using parts ≥ 5 */
static size_t dp_cap  = 0;    /* allocated size of dp */
static idx_t  max_n   = 0;    /* largest n for which dp is valid */

seq_t a(idx_t n)
{
    /* When a larger n is requested, recompute the partition table up
       to that n.  Complexity is O(n²), which is fine for n ≤ 10 000. */
    if (n > max_n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > dp_cap) {
            seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
            if (!tmp) {        /* allocation failure fallback */
                return (seq_t)0;
            }
            dp = tmp;
            dp_cap = new_cap;
        }

        /* reset dp array */
        for (size_t i = 0; i <= (size_t)n; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* standard partition DP, parts start at 5 */
        for (idx_t k = 5; k <= n; ++k) {
            for (idx_t i = k; i <= n; ++i) {
                dp[i] += dp[i - k];
            }
        }
        max_n = n;
    }

    /* Sequence values: 0 for n < 5, otherwise dp[n] - 1 */
    if (n < 5)
        return (seq_t)0;
    return dp[n] - (seq_t)1;
}
