/*
 * A179356  Number of permutations of 1..n with i-9<=p(i)<=i+3.
 * https://oeis.org/A179356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations p of 1..n such that i-9 <= p(i) <=
 *            i+3 for all i. This is computed by a banded DP over the
 *            13‑column sliding window described above
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* for size_t, idx_t, seq_t */
#include <string.h>     /* for memset, memcpy */

#define MASK_BITS 13
#define NUM_MASKS (1 << MASK_BITS)
#define ALLONES ((1 << MASK_BITS) - 1)

seq_t a(idx_t n)
{
    static seq_t dp[NUM_MASKS];
    static seq_t newdp[NUM_MASKS];
    int init_mask = 0;

    /* initial mask for row 1: columns [1-9,1+3] = [-8,4] */
    for (int t = 0; t < MASK_BITS; ++t) {
        int col = (int)(1 - 9) + t;        /* column index for row 1 */
        if (col < 1 || (idx_t)col > n)
            init_mask |= 1 << t;           /* mark out‑of‑range columns as used */
    }

    memset(dp, 0, sizeof(dp));
    dp[init_mask] = 1;

    for (idx_t row = 1; row <= n; ++row) {
        /* zero the next DP array */
        memset(newdp, 0, sizeof(newdp));

        for (int mask = 0; mask < NUM_MASKS; ++mask) {
            seq_t ways = dp[mask];
            if (ways == 0) continue;

            /* bits that are still unused in this window */
            int avail = (~mask) & ALLONES;

            /* iterate over each unused bit */
            for (int t = 0; t < MASK_BITS; ++t) {
                if (!(avail & (1 << t))) continue;

                int mask2 = ((mask | (1 << t)) >> 1);
                if (row + 4 > n)
                    mask2 |= 1 << 12;            /* new column is out of bounds */

                newdp[mask2] += ways;
            }
        }

        /* copy newdp into dp for the next iteration */
        memcpy(dp, newdp, sizeof(dp));
    }

    return dp[ALLONES];   /* all columns must be used */
}
