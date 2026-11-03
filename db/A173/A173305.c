/*
 * A173305  Triangle by columns, A000009 in every column shifted down twice
 *          for k > 0.
 * https://oeis.org/A173305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 224, u16 n <= 1599, u32 n <= 10301, u64 n <= 10301,
 *            u128 n <= 10301, big any n
 * Validated: standard, pass, 97 terms through n = 8419 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mem.h"

/* A173305: Triangle by columns, A000009 in every column shifted down twice for k > 0.
   The sequence is obtained by writing the triangle
       T(n,k) = A000009(n-2k) for k=0..⌊n/2⌋
   row‑wise (first n=0, then n=1, n=2, ...) and flattening all rows.
   Each row n has length ⌊n/2⌋+1.
   The function computes a(n) = T(r,k) where r is the row containing position n
   and k = n - Σ_{i<r} (⌊i/2⌋+1).  */
#ifndef _A173305_H_
#define _A173305_H_

#include <stdlib.h>
#include <stddef.h>

/* Partition into distinct parts: A000009 */
static seq_t *dp = NULL;
static const idx_t MAX_M = 2000;   /* sufficient for n up to ~10000 */

static void init_dp(void)
{
    if (dp != NULL)
        return;
    dp = mem_new(seq_t, (MAX_M + 1));
    if (!dp)
        exit(1); /* allocation failure */
    /* initialise to zero */
    for (idx_t i = 0; i <= MAX_M; ++i)
        dp[i] = 0;
    dp[0] = 1;
    /* coin change DP for partitions into distinct parts */
    for (idx_t part = 1; part <= MAX_M; ++part) {
        for (idx_t summ = MAX_M; summ >= part; --summ) {
            dp[summ] += dp[summ - part];
        }
    }
}

seq_t a(idx_t n)
{
    init_dp();

    /* find the row r such that n lies in that row */
    idx_t cum  = 0;   /* number of elements before row r */
    idx_t r    = 0;
    for (;;) {
        idx_t len = (r / 2) + 1;          /* length of row r */
        if (cum + len > n)
            break;
        cum += len;
        ++r;
    }

    /* position within the row */
    idx_t offset = n - cum;       /* 0‑based index inside row r */
    idx_t k      = offset;        /* column index k */

    /* compute the value using A000009 */
    idx_t m = r - 2 * k;          /* argument of A000009 (always non‑negative) */
    if (m > MAX_M) {
        /* In case n is much larger than expected, extend DP dynamically */
        idx_t old_max = MAX_M;
        idx_t new_max = m + 100; /* add a safety margin */
        seq_t *new_dp = mem_resize(seq_t, dp, (new_max + 1));
        if (!new_dp)
            exit(1);
        dp = new_dp;
        /* initialise newly added entries */
        for (idx_t i = old_max + 1; i <= new_max; ++i)
            dp[i] = 0;
        /* extend DP */
        for (idx_t part = old_max + 1; part <= new_max; ++part) {
            for (idx_t summ = new_max; summ >= part; --summ) {
                dp[summ] += dp[summ - part];
            }
        }
    }

    return dp[m];
}

#endif /* _A173305_H_ */
