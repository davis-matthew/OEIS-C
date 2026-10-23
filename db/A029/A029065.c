/*
 * A029065  Expansion of 1/((1-x)*(1-x^4)*(1-x^5)*(1-x^8)).
 * https://oeis.org/A029065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 4, 5, and 8
 * Range:     u8 n <= 53, u16 n <= 388, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* Allocate an array of size n+1 to hold the DP table. */
    seq_t *dp = alloc_seq(n + 1);

    /* Initialise the table: only a(0) = 1, others zero. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* The only allowed part sizes are 1, 4, 5, and 8. */
    const idx_t parts[4] = {1, 4, 5, 8};

    /* For each part size, update the DP table. */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t part = parts[pi];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free_seq(dp);
    return result;
}
