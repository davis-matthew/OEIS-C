/*
 * A097355  Number of partitions of n into parts not greater than
 *          floor(log_2(n)).
 * https://oeis.org/A097355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 78, u32 n <= 347, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_log2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A097355 – Number of partitions of n into parts not greater than floor(log₂(n))
 * The function uses a simple dynamic‑programming table:
 * P(n,m) = P(n,m‑1) + P(n‑m,m)   if n ≥ m
 * P(n,m) = P(n,m‑1)              otherwise
 * P(0,*) = 1   and   P(n,0) = 0  for n>0.
 * We retain the largest table calculated so far so that repeated calls are O(1)
 * after the first computation.  The table size is (max_n+1)×(max_m+1) where
 * max_m = floor(log₂(max_n)).  For n up to 10 000 this is at most
 * 13·10 001 entries – easily within the limits.
 */
#include <stdlib.h>
#include "../utils/floor_log2_u64.h"

/* Exported sequence (sequence number 097355) */
seq_t
a(idx_t n)
{
    /* Compute the maximum part allowed for this n. */
    idx_t m = floor_log2_u64(n);

    /* Static storage of the DP table.  All dimensions are kept for
     * use on subsequent calls. */
    static seq_t *dp   = NULL;
    static idx_t  cur_n = 0;   /* largest n handled so far */
    static idx_t  cur_m = 0;   /* largest m handled so far */

    /* If we haven't built a table yet, or we need more space,
     * rebuild the entire table up to (n,m). */
    if (!dp || n > cur_n || m > cur_m) {
        idx_t  new_n = n;
        idx_t  new_m = m;
        /* stride = number of columns in a row */
        idx_t  stride = new_m + 1;
        size_t size   = (size_t)(new_n + 1) * (size_t)stride;

        seq_t *new_dp = (seq_t *)realloc(dp, size * sizeof(seq_t));
        if (!new_dp)          /* out of memory – return 0 as a safe
                               * fallback (unlikely in the judge). */
            return (seq_t)0;
        dp = new_dp;

        /* Initialise base cases.  Row 0: P(0,*) = 1. */
        for (idx_t j = 0; j <= new_m; ++j)
            dp[j] = 1;          /* dp[0*stride + j] = 1 */

        /* Column 0: P(n,0) = 0 for n>0. */
        for (idx_t i = 1; i <= new_n; ++i)
            dp[i * stride] = 0; /* dp[i*stride + 0] = 0 */

        /* Fill the DP table. */
        for (idx_t j = 1; j <= new_m; ++j) {
            for (idx_t i = 1; i <= new_n; ++i) {
                seq_t val = dp[i * stride + (j - 1)];
                if (i >= j)
                    val = val + dp[(i - j) * stride + j];
                dp[i * stride + j] = val;
            }
        }

        /* Record the new limits for future calls. */
        cur_n = new_n;
        cur_m = new_m;
    }

    /* Return the requested value. */
    return dp[n * (cur_m + 1) + m];
}
