/*
 * A238914  Number of self-inverse permutations p on [n] where the maximal
 *          displacement of an element equals 3.
 * https://oeis.org/A238914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of self‑inverse permutations p on [n] whose
 *            maximal displacement equals 3. Computed by counting matchings on
 *            the graph with vertices 1..n and edges i–j if |i-j|≤3, keeping
 *            track of whether a distance‑3 edge has been used
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t dp[8][2];
    static seq_t ndp[8][2];

    /* initialise dp */
    for (int m = 0; m < 8; ++m)
        for (int u = 0; u < 2; ++u)
            dp[m][u] = 0;
    dp[0][0] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        /* zero ndp */
        for (int m = 0; m < 8; ++m)
            for (int u = 0; u < 2; ++u)
                ndp[m][u] = 0;

        for (int mask = 0; mask < 8; ++mask) {
            for (int used = 0; used < 2; ++used) {
                seq_t val = dp[mask][used];
                if (!val) continue;

                /* leave i unmatched */
                int nextMask = (((mask << 1) | 1) & 7);
                ndp[nextMask][used] += val;

                /* match with i‑1 (distance 1) */
                if (mask & 1) {
                    int tmp = mask & ~1;
                    nextMask = ((tmp << 1) & 7);
                    ndp[nextMask][used] += val;
                }

                /* match with i‑2 (distance 2) */
                if (mask & 2) {
                    int tmp = mask & ~2;
                    nextMask = ((tmp << 1) & 7);
                    ndp[nextMask][used] += val;
                }

                /* match with i‑3 (distance 3) */
                if (mask & 4) {
                    int tmp = mask & ~4;
                    nextMask = ((tmp << 1) & 7);
                    ndp[nextMask][1] += val;   /* used becomes true */
                }
            }
        }

        /* copy ndp to dp */
        for (int m = 0; m < 8; ++m)
            for (int u = 0; u < 2; ++u)
                dp[m][u] = ndp[m][u];
    }

    /* sum over final states where at least one distance‑3 edge was used */
    seq_t ans = 0;
    for (int mask = 0; mask < 8; ++mask)
        ans += dp[mask][1];

    return ans;
}
