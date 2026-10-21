/*
 * A096914  Number of partitions of 2*n into distinct parts with exactly two
 *          odd parts.
 * https://oeis.org/A096914
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of 2*n into distinct parts with
 *            exactly two odd parts (OEIS A096914)
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global state: DP table dp[sum][odd] where odd = 0,1,2.
   dp is laid out as a single array of length (maxSum+1)*3. */
static seq_t *dp = NULL;
static idx_t dp_max = 0;  /* largest sum for which dp[] is valid */

/* Build the DP table for all sums up to maxSum.
   When dp_max >= maxSum the table is already ready. */
static void build_dp(idx_t maxSum)
{
    if (dp != NULL && dp_max >= maxSum)
        return;

    /* Allocate new table and zero‑initialize it. */
    idx_t newSize = maxSum + 1;
    seq_t *newDp = (seq_t *)calloc(newSize * 3, sizeof(seq_t));
    if (!newDp)
        return;  /* out of memory – the function will return 0 */

    /* Empty partition for sum 0, 0 odd parts. */
    newDp[0] = 1;

    /* DP: iterate over every possible part value. */
    for (idx_t part = 1; part <= maxSum; ++part) {
        idx_t partOdd = part & 1;  /* 0 if even, 1 if odd */
        for (idx_t sum = maxSum; sum >= part; --sum) {
            for (idx_t odd = 0; odd <= 2 - partOdd; ++odd) {
                seq_t val = newDp[(sum - part) * 3 + odd];
                if (val) {  /* skip zero adds for speed */
                    newDp[sum * 3 + odd + partOdd] += val;
                }
            }
        }
    }

    /* Replace the old table. */
    free(dp);
    dp = newDp;
    dp_max = maxSum;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* offset starts at 2 */

    idx_t maxSum = 2 * n;
    build_dp(maxSum);

    return dp[maxSum * 3 + 2];   /* exactly two odd parts */
}
