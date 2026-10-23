/*
 * A029016  Expansion of 1/((1-x)*(1-x^2)*(1-x^5)*(1-x^12)).
 * https://oeis.org/A029016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 351, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = number of partitions of n using parts 1, 2, 5, and 12.
   Computed by dynamic programming with a 1‑dimensional array. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq.h"      /* alloc_seq() */
#include "../utils/free_seq.h"       /* free_seq() */

seq_t a(idx_t n)
{
    /* handle the zero case directly */
    if (n == 0)
        return (seq_t)1;

    /* Allocate dp[0 .. n] */
    seq_t *dp = alloc_seq((size_t)(n + 1));
    if (!dp)
        return (seq_t)0;                /* allocation failure fallback */

    /* Zero‑initialize dp[1..n]; dp[0] already zeroed by alloc_seq() but set it explicitly. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Parts that may be used */
    const idx_t parts[4] = {1, 2, 5, 12};

    for (size_t pi = 0; pi < 4; ++pi) {
        idx_t coin = parts[pi];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] = dp[i] + dp[i - coin];
    }

    seq_t result = dp[n];
    free_seq(dp);
    return result;
}
