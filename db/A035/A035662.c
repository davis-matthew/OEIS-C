/*
 * A035662  Number of partitions of n into parts 7k+2 and 7k+3 with at least
 *          one part of each type.
 * https://oeis.org/A035662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #partitions of n into parts 7k+2 and 7k+3 * with at least
 *            one part of each type
 * Range:     u8 n <= 52, u16 n <= 135, u32 n <= 408, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, free */

static void compute_dp(idx_t N, seq_t *dp_all, seq_t *dp2, seq_t *dp3)
{
    /* Initialize all entries to 0, except the empty partition */
    for (idx_t i = 0; i <= N; ++i) {
        dp_all[i] = 0;
        dp2[i] = 0;
        dp3[i] = 0;
    }
    dp_all[0] = dp2[0] = dp3[0] = 1;

    /* Add contributions from parts of the form 7k+2 */
    for (idx_t coin = 2; coin <= N; coin += 7) {
        for (idx_t i = coin; i <= N; ++i) {
            dp_all[i] += dp_all[i - coin];
            dp2[i] += dp2[i - coin];
        }
    }

    /* Add contributions from parts of the form 7k+3 */
    for (idx_t coin = 3; coin <= N; coin += 7) {
        for (idx_t i = coin; i <= N; ++i) {
            dp_all[i] += dp_all[i - coin];
            dp3[i] += dp3[i - coin];
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)           /* sequence starts at offset 1 */
        return 0;

    /* Static tables that grow as larger n are requested. */
    static seq_t *dp_all = NULL;
    static seq_t *dp2   = NULL;
    static seq_t *dp3   = NULL;
    static idx_t maxN   = 0;

    /* Grow the tables if needed.  Each expansion recomputes all tables
     * from scratch for the new upper bound.  Since the number of coins
     * grows linearly with n, this O(n²) algorithm is fast enough for
     * n ≤ 10000 in the 2‑second limit. */
    if (n > maxN) {
        size_t sz = (size_t)(n + 1);
        seq_t *new_all = (seq_t *)malloc(sz * sizeof(seq_t));
        seq_t *new2    = (seq_t *)malloc(sz * sizeof(seq_t));
        seq_t *new3    = (seq_t *)malloc(sz * sizeof(seq_t));

        if (!new_all || !new2 || !new3) {   /* allocation failed */
            /* In the unlikely event of out‑of‑memory, fall back to 0. */
            return 0;
        }

        if (dp_all) {            /* free old memory if present */
            free(dp_all);
            free(dp2);
            free(dp3);
        }

        dp_all = new_all;
        dp2   = new2;
        dp3   = new3;
        maxN  = n;

        compute_dp(maxN, dp_all, dp2, dp3);
    }

    /* Partitions that contain at least one part of each type: */
    seq_t result = dp_all[n] - dp2[n] - dp3[n];
    return result;
}
