/*
 * A079993  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={0,2}.
 * https://oeis.org/A079993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of permutations p of {1..n} with -3 <= p(i)-i <= 3 * and
 *            p(i)-i not in {0,2} for all 1 <= i <= n. OEIS sequence A079993.
 *            * * This implementation uses a sliding‑window DP with a 7‑bit
 *            mask that keeps * track of which of the columns
 *            {i-3,i-2,i-1,i,i+1,i+2,i+3} are already * occupied by earli...
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   mask_accept
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mask_accept.h"

static const int OFFSETS[5] = { -3, -2, -1,  1,  3 };

seq_t a(idx_t n)
{
    if (n == 0) return 1;               /* a(0) = 1 */

    /* DP arrays: 128 possible masks (7 bits) */
    seq_t dp[128]       = {0};
    seq_t nextdp[128]   = {0};

    dp[0] = 1;                           /* before the first row all columns are free */

    for (idx_t i = 1; i <= n; ++i) {
        /* zero next dp array for the current step */
        for (int m = 0; m < 128; ++m) nextdp[m] = 0;

        int ii = (int)i;                 /* current row index as signed int */

        for (int m = 0; m < 128; ++m) {
            seq_t cur = dp[m];
            if (cur == 0) continue;      /* skip empty states */

            for (int k = 0; k < 5; ++k) {
                int jj = ii + OFFSETS[k];
                if (jj < 1 || jj > (int)n) continue;   /* target column out of bounds */

                int bitpos = OFFSETS[k] + 3;           /* map offset to bit position 0..6 */

                if (m & (1 << bitpos)) continue;       /* column already used */

                int new_mask = m | (1 << bitpos);

                if (i == n)                         /* last row – keep mask unchanged */
                    nextdp[new_mask] += cur;
                else                                /* shift right for the next row */
                    nextdp[new_mask >> 1] += cur;
            }
        }

        if (i != n) {                          /* prepare for next iteration */
            for (int m = 0; m < 128; ++m) dp[m] = nextdp[m];
        }
    }

    /* After processing all rows, sum all states that satisfy the completeness test */
    seq_t ans = 0;
    for (int m = 0; m < 128; ++m)
        if (mask_accept(n, m))
            ans += nextdp[m];

    return ans;
}
