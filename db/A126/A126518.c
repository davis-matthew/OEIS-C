/*
 * A126518  Number of base 23 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Number of length-n base-23 numbers with adjacent digits differing by at most 4.
    Recurrence:  a(0) = 1.
    Let dp[d] be the number of n‑digit strings ending with digit d.
    Initialize for length 1: dp[d] = 1 for all 0 ≤ d < 23.
    For each subsequent length:
        next[d'] = Σ_{d: |d−d'| ≤ 4} dp[d]
    After processing length n, a(n) = Σ_{d=0}^{22} dp[d].
    Complexity: O(n·23·9) time, O(23) space. */

#include <stddef.h>    /* for NULL */
#include <stdlib.h>    /* for size_t, malloc, free */

seq_t a(idx_t n)
{
    /* a(0) = 1 (empty string) */
    if (n == 0) return (seq_t)1;

    /* dp and next hold counts for each last digit */
    seq_t dp[23];
    seq_t next[23];

    /* length 1: all 23 digits are possible */
    for (int d = 0; d < 23; ++d) dp[d] = (seq_t)1;

    /* extend strings up to length n */
    for (idx_t len = 1; len < n; ++len) {
        /* zero next array */
        for (int d = 0; d < 23; ++d) next[d] = (seq_t)0;

        /* transition: for each current last digit, add to allowed next digits */
        for (int d = 0; d < 23; ++d) {
            seq_t cur = dp[d];
            if (cur == 0) continue;
            /* allowed next digits are within ±4 */
            for (int k = -4; k <= 4; ++k) {
                int nd = d + k;
                if (nd >= 0 && nd < 23) next[nd] += cur;
            }
        }

        /* swap dp with next for next iteration */
        for (int d = 0; d < 23; ++d) dp[d] = next[d];
    }

    /* sum all endings to get total count */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 23; ++d) total += dp[d];

    return total;
}
