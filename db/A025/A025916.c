/*
 * A025916  Expansion of 1/((1-x^7)*(1-x^10)*(1-x^12)).
 * https://oeis.org/A025916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 7, 10, and 12 (OEIS
 *            A025916)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t max_n = 0;          /* highest n for which dp[0..max_n] is computed */

    /* Recompute DP if necessary */
    if (dp == NULL || n > max_n)
    {
        /* Allocate array for dp[0..n] */
        size_t len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)malloc(len * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failure – abort */

        /* Initialize */
        tmp[0] = 1;
        for (idx_t i = 1; i <= n; ++i) tmp[i] = 0;

        /* DP over parts 7, 10, 12 – outer loop over parts ensures combinations only */
        const idx_t parts[3] = {7, 10, 12};
        for (int pidx = 0; pidx < 3; ++pidx) {
            idx_t p = parts[pidx];
            for (idx_t i = p; i <= n; ++i) {
                tmp[i] += tmp[i - p];
            }
        }

        /* Replace old cache */
        free(dp);
        dp = tmp;
        max_n = n;
    }

    return dp[n];
}
