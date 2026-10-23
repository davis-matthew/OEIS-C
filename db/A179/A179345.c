/*
 * A179345  Number of permutations of 1..n with i-7<=p(i)<=i+3.
 * https://oeis.org/A179345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of 1..n with i-7 <= p(i) <= i+3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* window parameters */
    const int LEFT  = 7;           /* allowed positions to the left */
    const int RIGHT = 3;           /* allowed positions to the right */
    const int W     = LEFT + RIGHT + 1;             /* total width = 11 */
    const int MASKS = 1 << W;                       /* 2048 masks */
    const unsigned FULL_MASK = MASKS - 1;           /* all‑1 mask of width W */

    /* DP arrays */
    seq_t *dp  = (seq_t*)calloc(MASKS, sizeof(seq_t));
    seq_t *ndp = (seq_t*)calloc(MASKS, sizeof(seq_t));
    if (!dp || !ndp) {
        free(dp); free(ndp);
        return (seq_t)0;
    }

    /* Precompute bound masks for positions i = 1 .. n+1
       bound[i] is a mask having a 1 bit for every column of the window
       [i-LEFT , i+RIGHT] that lies outside 1..n.                      */
    uint32_t *bound = (uint32_t*)malloc((n + 2) * sizeof(uint32_t));
    if (!bound) {
        free(dp); free(ndp);
        return (seq_t)0;
    }
    for (idx_t i = 1; i <= n + 1; ++i) {
        uint32_t m = 0;
        int start_col = (int)i - LEFT;
        for (int k = 0; k < W; ++k) {
            int col = start_col + k;
            if (col < 1 || col > (int)n)
                m |= 1u << k;
        }
        bound[i] = m;
    }

    /* Initial state: window for i = 1, columns outside 1..n are already
       considered occupied. */
    uint32_t init_mask = bound[1];
    dp[init_mask] = (seq_t)1;

    /* Main DP */
    for (idx_t i = 1; i <= n; ++i) {
        uint32_t bound_next = bound[i + 1];
        /* zero ndp */
        memset(ndp, 0, MASKS * sizeof(seq_t));

        for (int mask = 0; mask < MASKS; ++mask) {
            seq_t ways = dp[mask];
            if (!ways)
                continue;

            /* free columns in the current window (bit 0 means free) */
            uint32_t avail = (~mask) & FULL_MASK;
            while (avail) {
                uint32_t bit = avail & -avail;   /* lowest set bit */
                avail -= bit;

                uint32_t new_mask = mask | bit;
                uint32_t next_mask = (new_mask >> 1) | bound_next;
                ndp[next_mask] += ways;
            }
        }
        /* swap dp and ndp for next iteration */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    seq_t result = dp[FULL_MASK];

    /* clean up */
    free(dp);
    free(ndp);
    free(bound);
    return result;
}
