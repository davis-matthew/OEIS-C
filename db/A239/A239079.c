/*
 * A239079  Number of self-inverse permutations p on [n] with displacement of
 *          elements restricted by 7: |p(i)-i| <= 7.
 * https://oeis.org/A239079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of involutive permutations p on [n] with |p(i)-i|
 *            ≤ 7. Computation is via a linear DP that tracks, for each
 *            prefix, which of the next 7 positions are already matched by an
 *            edge originating earlier. The state is a 7‑bit mask; the
 *            recurrence uses a sliding window over the n vertices
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

enum { OFF = 7, STATES = 1 << OFF };

seq_t a(idx_t n)
{
    static seq_t dp[2][STATES];
    int cur = 0, nxt = 1;

    /* initialize dp */
    for (int s = 0; s < STATES; ++s) dp[cur][s] = 0;
    dp[cur][0] = 1;   /* empty prefix */

    for (idx_t pos = 0; pos < n; ++pos) {
        /* zero next array */
        for (int s = 0; s < STATES; ++s) dp[nxt][s] = 0;

        for (int mask = 0; mask < STATES; ++mask) {
            seq_t ways = dp[cur][mask];
            if (!ways) continue;

            /* if bit 0 is set, current vertex already matched */
            if (mask & 1) {
                int nextMask = mask >> 1;
                dp[nxt][nextMask] += ways;
                continue;
            }

            /* option 1: keep current vertex as a fixed point */
            dp[nxt][mask >> 1] += ways;

            /* option 2: pair current vertex with a future vertex */
            for (int k = 1; k <= OFF; ++k) {
                idx_t j = pos + (idx_t)k;
                if (j >= n) break;
                if ((mask >> k) & 1) continue;    /* future vertex already matched */
                int newMask = (mask >> 1) | (1 << (k - 1));
                dp[nxt][newMask] += ways;
            }
        }

        cur ^= 1;
        nxt ^= 1;
    }

    return dp[cur][0];
}
