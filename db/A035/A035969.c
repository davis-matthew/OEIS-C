/*
 * A035969  Number of partitions of n into parts not of the form 17k, 17k+8 or
 *          17k-8. Also number of partitions with at most 7 parts of size 1
 *          and differences between parts at distance 7 are greater than 1.
 * https://oeis.org/A035969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into * parts not congruent to
 *            0, 8, or 9 modulo 17. * Computed by a standard
 *            unbounded‑knapsack DP
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 143, u64 n <= 478,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 995 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

static inline int allowed_part(idx_t part)
{
    int r = part % 17;
    return r != 0 && r != 8 && r != 9;
}

seq_t a(idx_t n)
{
    /* Allocate a DP table dp[0..n] as an array of seq_t. */
    seq_t *dp = alloc_seq(n + 1);
    dp[0] = (seq_t)1;          /* the empty partition */

    /* For every allowed part size k, add its contribution. */
    for (idx_t k = 1; k <= n; ++k) {
        if (!allowed_part(k))
            continue;
        for (idx_t j = k; j <= n; ++j) {
            dp[j] += dp[j - k];
        }
    }

    seq_t result = dp[n];
    /* Memory allocated by alloc_seq is intentionally not freed
     * – the calling framework can reclaim it if necessary. */
    return result;
}
