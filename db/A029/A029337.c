/*
 * A029337  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 116, u16 n <= 814, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of partitions of n into parts 4, 5, 8, and 9.
    The value is computed by an unbounded coin‑change dynamic programming
    algorithm with O(4·n) time and O(n) space.  The function allocates a
    temporary array on each call, which is freed before returning.  This
    implementation works for any unsigned integer type seq_t and for any
    integral index type idx_t (typically uint64_t).  It does not rely on
    external utilities and is valid C11 and C++17. */
#include <stddef.h>   /* for NULL */
#include <stdlib.h>   /* for calloc, free */

seq_t a(idx_t n)
{
    /* Allocate array of size n+1, zero‑initialized */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;            /* allocation failed – unlikely for n ≤ 10000 */

    dp[0] = 1;                    /* one way to form sum 0 */

    /* allowed parts */
    const idx_t coins[4] = {4, 5, 8, 9};

    for (int ci = 0; ci < 4; ++ci) {
        idx_t c = coins[ci];
        for (idx_t i = c; i <= n; ++i) {
            dp[i] += dp[i - c];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
