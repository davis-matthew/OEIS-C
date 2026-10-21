/*
 * A069713  As a square array T(n,k) by antidiagonals, number of ways of
 *          partitioning k into up to n parts each no more than 5, or into up
 *          to 5 parts each no more than n; as a triangle t(n,k), number of
 *          ways of partitioning n into exactly k parts each no more than 6
 *          (i.e., of arranging k indistinguishable st...
 * https://oeis.org/A069713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A069713 is obtained by reading the square array T(n,k) – the
 *            number of partitions of k into at most n parts each not
 *            exceeding 5 – row‑by‑row (anti‑diagonal in the OEIS definition).
 *            The sequence is therefore a(0)=T(0,0), a(1)=T(0,1), a(2)=T(1,1),
 *            a(3)=T(0,2), … The implementation uses a dynamic p...
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   row_from_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/row_from_index.h"

/* dp[sum] is a pointer to an array of length sum+1. */
static seq_t **dp = NULL;
static idx_t    dp_rows = 0;   /* largest sum for which dp is built */

/* free the current dp table */
static void free_dp(void)
{
    if (dp == NULL)
        return;
    for (idx_t i = 0; i <= dp_rows; ++i)
        free(dp[i]);
    free(dp);
    dp = NULL;
    dp_rows = 0;
}

/* build dp for sums 0..maxR (inclusive) */
static void recompute_dp(idx_t maxR)
{
    /* free previous table */
    free_dp();

    /* allocate new table */
    dp = (seq_t **)malloc((maxR + 1) * sizeof(seq_t *));
    for (idx_t i = 0; i <= maxR; ++i)
        dp[i] = (seq_t *)calloc((size_t)(i + 1), sizeof(seq_t));

    /* base case */
    dp[0][0] = 1;

    /* DP: add part values 1..6 (unordered, unlimited supply) */
    for (idx_t val = 1; val <= 6; ++val) {
        for (idx_t sum = val; sum <= maxR; ++sum) {
            for (idx_t parts = 1; parts <= sum; ++parts) {
                if (parts - 1 > sum - val)  /* impossible remainder */
                    continue;
                dp[sum][parts] += dp[sum - val][parts - 1];
            }
        }
    }

    dp_rows = maxR;
}

/* required function */
seq_t a(idx_t n)
{
    /* find (sum,parts) for index n */
    idx_t sum = row_from_index(n);
    unsigned __int128 tri = (unsigned __int128)sum * (sum + 1) / 2;
    idx_t parts = n - (idx_t)tri;

    /* ensure dp table covers required sum */
    if (dp == NULL || sum > dp_rows)
        recompute_dp(sum);

    /* parts will never exceed sum by construction, but guard anyway */
    if (parts > sum)
        return (seq_t)0;

    return dp[sum][parts];
}
