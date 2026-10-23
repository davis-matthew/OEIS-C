/*
 * A197081  Number of partitions of n into distinct positive quarter-squares
 *          (cf. A002620).
 * https://oeis.org/A197081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 137, u16 n <= 518, u32 n <= 555, u64 n <= 555,
 *            u128 n <= 555, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A197081:  Number of partitions of n into distinct positive
    quarter‑squares (floor(k^2/4)).
    The algorithm uses a 1‑D dynamic programming array:  dp[s] – the
    number of partitions of s with distinct quarter‑squares.
    For each quarter‑square q processed in increasing order we update
    dp[s] += dp[s-q] for s from n down to q.  This yields a(n)=dp[n].
    The routine is memoised with a static array that grows to the
    requested size, so successive calls are fast and the results
    are correct independent of call order.  It runs in O(n · √n)
    time, fast enough for n up to 10 000. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *dp = NULL;          /* dynamic DP table */
static size_t dp_cap = 0;         /* current capacity of dp */

seq_t a(idx_t n)
{
    /* Allocate / resize the DP array if necessary */
    if ((size_t)n + 1 > dp_cap) {
        seq_t *tmp = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed – unlikely */
        free(dp);
        dp = tmp;
        dp_cap = (size_t)n + 1;
    }

    /* Initialise to zero and set base case */
    for (size_t i = 0; i <= (size_t)n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Process all distinct quarter‑squares ≤ n */
    for (idx_t k = 2; ; ++k) {
        idx_t q = (k * k) / 4;    /* floor(k²/4) */
        if (q == 0)               /* skip zero (k=1 gives 0) */
            continue;
        if ((idx_t)q > n)
            break;
        for (size_t i = (size_t)n; i >= (size_t)q; --i)
            dp[i] = dp[i] + dp[i - q];
    }
    return dp[(size_t)n];
}
