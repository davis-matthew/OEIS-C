/*
 * A103344  Number of representations of n as a sum of distinct elements of
 *          the Fibonacci-type sequence beginning 1, 4, 5, 9, 14, 23, 37, 60,
 *          ....
 * https://oeis.org/A103344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct subsets of the Fibonacci‑type sequence
 *            (1,4,5,9,14,23,…) that sum to n. Computed by dynamic programming
 * Range:     u8 n <= 2281, u16 n <= 9938, u32 n <= 9938, u64 n <= 9938,
 *            u128 n <= 9938, big any n
 * Helpers:   seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_malloc.h"
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Pre‑compute Fibonacci‑type numbers not exceeding n. */
    seq_t fibs[64];
    fibs[0] = (seq_t)1;
    fibs[1] = (seq_t)4;
    idx_t sz = 2;
    while (1) {
        seq_t next = fibs[sz-1] + fibs[sz-2];
        if (next > n) break;
        fibs[sz++] = next;
        if (sz == 64) break;          /* safety guard, very unlikely */
    }

    /* dp[s] = number of subsets summing to s. */
    idx_t dp_size = n + 1;
    seq_t *dp = seq_malloc(dp_size);
    if (!dp) return (seq_t)0;
    for (idx_t i = 0; i < dp_size; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t i = 0; i < sz; ++i) {
        const seq_t val = fibs[i];
        idx_t v = (idx_t)val;          /* val <= n for the indices we process */
        for (idx_t s = n; s >= v; --s) {
            dp[s] += dp[s - v];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
