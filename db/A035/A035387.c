/*
 * A035387  Number of partitions of n into parts 6k+3 or 6k+4.
 * https://oeis.org/A035387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into parts of the form 6k+3 or 6k+4
 * Range:     u8 n <= 56, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* dynamic array storing partition counts */
static idx_t dp_len = 0;          /* number of elements in dp (dp_len = max_n + 1) */

seq_t a(idx_t n)
{
    /* Ensure n is at least 1; values for n < 1 are defined as 0 */
    if (n < 1) return 0;

    /* Grow the dynamic array if we need to compute up to a larger n */
    if (dp_len <= n) {
        idx_t new_len = n + 1;
        seq_t *newdp = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!newdp) abort();          /* fallback on allocation failure */

        /* initialize to zero */
        for (idx_t i = 0; i < new_len; ++i) newdp[i] = 0;
        newdp[0] = 1;                 /* base case: one way to partition 0 */

        /* Dynamic programming: unbounded knapsack over allowed parts */
        for (idx_t part = 3; part <= n; ++part) {
            if (part % 6 == 3 || part % 6 == 4) {
                for (idx_t sum = part; sum <= n; ++sum) {
                    newdp[sum] += newdp[sum - part];
                }
            }
        }

        /* Replace old array */
        free(dp);
        dp      = newdp;
        dp_len  = new_len;
    }

    return dp[n];
}
