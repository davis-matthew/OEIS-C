/*
 * A035367  Number of partitions of n into parts 5k or 5k+1.
 * https://oeis.org/A035367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into parts 5k or 5k+1
 * Range:     u8 n <= 39, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* dynamic table of partition counts */
static idx_t dp_size = 0;     /* current allocated size of dp */

/* Recompute the partition counts up to n (inclusive). */
static void recompute_up_to(idx_t n)
{
    if (n + 1 <= dp_size)
        return;

    idx_t new_size = n + 1;
    seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!new_dp)
        return; /* allocation failed – caller will return garbage, as allowed */

    dp = new_dp;
    /* zero the array first */
    for (idx_t i = 0; i < new_size; ++i)
        dp[i] = 0;

    dp[0] = 1;  /* one way to partition 0 */

    /* iterate over all allowed part values */
    for (idx_t c = 1; c < new_size; ++c) {
        if (c % 5 == 0 || c % 5 == 1) {
            for (idx_t i = c; i < new_size; ++i)
                dp[i] += dp[i - c];
        }
    }
    dp_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for offset < 1, but never called with n<1 */

    recompute_up_to(n);
    return dp[n];
}
