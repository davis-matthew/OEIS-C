/*
 * A072853  Number of permutations satisfying i-2<=p(i)<=i+6, i=1..n.
 * https://oeis.org/A072853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of permutations p of 1..n such that for all i, i‑2
 *            <= p(i) <= i+6. The function uses a dynamic programming state of
 *            width 9 (the window of admissible values) and runs in O(9·512·n)
 *            time. The recurrence is not expressed explicitly; instead a
 *            sliding–window DP is performed
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const int W = 9;
static const unsigned int MASK_ALL = (1U << W) - 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe */

    /* Allocate the array of invalid‑bit masks for window starts 1..n. */
    unsigned *invalid = (unsigned*)malloc((size_t)(n + 2) * sizeof(unsigned));
    if (!invalid) return 0;

    /* Pre‑compute the invalid mask for each position i. */
    for (idx_t i = 1; i <= n; ++i) {
        int start = (int)i - 2;
        unsigned m = 0;
        for (int j = 0; j < W; ++j) {
            int val = start + j;
            if (val < 1 || val > (int)n)
                m |= (1U << j);
        }
        invalid[i] = m;
    }

    /* DP arrays (size 512 = 2^9).  They are zeroed each call. */
    seq_t dp[1 << W], dp_next[1 << W];
    memset(dp, 0, sizeof(dp));
    memset(dp_next, 0, sizeof(dp_next));

    /* Initial state: before processing position 1. */
    dp[invalid[1]] = 1;

    seq_t ans = 0;

    for (idx_t pos = 1; pos <= n; ++pos) {
        memset(dp_next, 0, sizeof(dp_next));

        int start = (int)pos - 2;
        unsigned inv_next = (pos < n) ? invalid[pos + 1] : 0;

        for (unsigned mask = 0; mask < (1U << W); ++mask) {
            seq_t cur = dp[mask];
            if (cur == 0) continue;

            for (int j = 0; j < W; ++j) {
                if (mask & (1U << j)) continue;   /* value already used */

                int val = start + j;
                if (val < 1 || val > (int)n) continue;   /* safety */

                unsigned mask_used = mask | (1U << j);
                /* the leftmost value (i-2) must be used before it drops out */
                if ((mask_used & 1U) == 0) continue;

                if (pos == n) {
                    /* After the last position all bits must be set. */
                    if (mask_used == MASK_ALL)
                        ans += cur;
                } else {
                    unsigned shifted = mask_used >> 1;
                    unsigned new_mask = shifted | inv_next;
                    dp_next[new_mask] += cur;
                }
            }
        }
        /* Prepare for the next position. */
        memcpy(dp, dp_next, sizeof(dp));
    }

    free(invalid);
    return ans;
}
