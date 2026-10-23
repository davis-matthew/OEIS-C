/*
 * A108456  Table read by antidiagonals: T(n,k) = number of partitions of
 *          (n,k) into pairs (i,j) with i>0, j>=0.
 * https://oeis.org/A108456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where i = n - d*(d+1)/2, j = d - i and d =
 *            floor((sqrt(8n+1)-1)/2). Here T(i,j) counts partitions of (i,j)
 *            into pairs (s,t) with s>0, t>=0. The table of T is computed once
 *            by dynamic programming, and then a(n) reads the appropriate
 *            entry
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   triangular_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/triangular_index.h"

static const idx_t MAX = 200;                 /* sufficient for n <= 10000 */
static seq_t *dp = NULL;
static idx_t dp_size = 0;

/* Build the 2‑D table of T(i,j) for 0 <= i,j <= MAX */
static void init_dp(void)
{
    if (dp != NULL)
        return;

    dp_size = (MAX + 1) * (MAX + 1);
    dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
    for (idx_t idx = 0; idx < dp_size; ++idx)
        dp[idx] = 0;
    dp[0] = (seq_t)1;                      /* dp[0][0] = 1 */

    for (idx_t s = 1; s <= MAX; ++s) {
        for (idx_t t = 0; t <= MAX; ++t) {
            for (idx_t i = s; i <= MAX; ++i) {
                idx_t base = i * (MAX + 1);
                for (idx_t j = t; j <= MAX; ++j) {
                    dp[base + j] += dp[(i - s) * (MAX + 1) + (j - t)];
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    init_dp();
    idx_t d = triangular_index(n);
    idx_t off = n - d * (d + 1) / 2;
    idx_t i = off;
    idx_t j = d - off;
    return dp[i * (MAX + 1) + j];
}
