/*
 * A089055  Solution to the non-squashing boxes problem (version 2).
 * https://oeis.org/A089055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the subsets S⊆{0,…,n} whose sorted elements
 *            w1<w2<…<wk satisfy w_i ≥ w_1+…+w_{i−1}. This is computed by
 *            dynamic programming over subset sums
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* dp[sum] = number of good subsets with total weight sum.  */
seq_t a(idx_t n)
{
    /* Every good subset has total weight ≤ 2·n (proof by induction). */
    idx_t max_sum = n * 2;
    size_t len = (size_t)(max_sum + 1);

    seq_t *dp = (seq_t*)calloc(len, sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* allocation failure → 0 */

    dp[0] = 1;   /* the empty subset */

    for (idx_t j = 0; j <= n; ++j) {
        /* Add element j to all existing subsets with sum ≤ j.  */
        for (idx_t s = j; ; --s) {
            size_t idx_sum = (size_t)(s + j);
            dp[idx_sum] += dp[(size_t)s];
            if (s == 0) break;
        }
    }

    seq_t result = 0;
    for (size_t s = 0; s < len; ++s)
        result += dp[s];

    free(dp);
    return result;
}
