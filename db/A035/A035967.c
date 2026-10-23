/*
 * A035967  Number of partitions of n into parts not of the form 17k, 17k+6 or
 *          17k-6. Also number of partitions with at most 5 parts of size 1
 *          and differences between parts at distance 7 are greater than 1.
 * https://oeis.org/A035967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A035967: Number of partitions of n into parts not of the form
 * 17k, 17k+6 or 17k-6.  The values are computed by the classic
 * unlimited‑coin partition DP:  a(0)=1;  a(n)=∑_{p allowed} a(n-p).
 * Implementation recomputes the whole table on each call (≈O(n²) but
 * fine for n≤10000); the table is cached and reused for larger
 * arguments.  All arithmetic is performed in seq_t with only integer
 * operators. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *dp = NULL;       /* memoised DP table */
static size_t dp_size = 0;     /* current allocated table size */

/* Compute a(n) for n ≥ 1.  The offset is 1. */
seq_t a(idx_t n)
{
    /* Guard against ridiculously large indices that do not fit into
     * the statically sized array.  The build does not support such
     * indices, so we just return 0. */
    if (n > (idx_t)SIZE_MAX) return 0;

    size_t target = (size_t)n;          /* index in the DP table */
    if (dp_size <= target) {            /* need a larger table */
        seq_t *new_dp = (seq_t*)realloc(dp,
                                         (target + 1) * sizeof(seq_t));
        if (new_dp == NULL) abort();    /* out of memory */
        dp = new_dp;
        dp_size = target + 1;
    }

    /* Reset table to zero.  dp[0] will be set to 1 below. */
    for (size_t i = 0; i <= target; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Run the coin‑change DP over allowed parts. */
    for (size_t p = 1; p <= target; ++p) {
        /* Skip forbidden parts: 0, 6, 11 (mod 17). */
        size_t r = p % 17;
        if (r == 0 || r == 6 || r == 11) continue;
        for (size_t j = p; j <= target; ++j) {
            dp[j] += dp[j - p];
        }
    }

    return dp[target];
}
