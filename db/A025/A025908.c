/*
 * A025908  Expansion of 1/((1-x^7)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A025908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into parts 7, 8, and 9 a(0) = 1; For
 *            each n ≥ 1, a(n) counts the number of solutions to 7·x + 8·y +
 *            9·z = n with non‑negative integers x, y, z. The implementation
 *            uses the classic coin‑change dynamic‑programming algorithm that
 *            iterates over each part size once, guarantee...
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;   /* memoised DP table */
static idx_t   dp_size = 0;/* size of dp (entries 0 … dp_size-1) */

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Re‑compute the DP table if the requested index is beyond the current table. */
    if (n >= dp_size)
    {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc(sizeof(seq_t) * new_size);
        if (!new_dp) return 0;                   /* allocation failure → 0 */

        /* Zero‑initialise and set the base case. */
        for (idx_t i = 0; i < new_size; ++i) new_dp[i] = 0;
        new_dp[0] = 1;                           /* only one way to partition 0 */

        /* The three allowed part sizes. */
        const idx_t parts[3] = {7, 8, 9};

        /* Classic coin‑change DP: for each part size we add ways that use that part. */
        for (int p_id = 0; p_id < 3; ++p_id)
        {
            idx_t part = parts[p_id];
            for (idx_t i = part; i <= n; ++i)
            {
                new_dp[i] += new_dp[i - part];
            }
        }

        /* Replace old table and free its memory. */
        if (dp) free(dp);
        dp = new_dp;
        dp_size = new_size;
    }

    return dp[n];
}
