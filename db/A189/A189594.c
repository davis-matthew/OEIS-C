/*
 * A189594  Number of permutations of 1..n with displacements restricted to
 *          {-6,-5,-4,-3,-1,0,2}.
 * https://oeis.org/A189594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 60, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic‑programming solution for OEIS A189594
 *
 * Count permutations of 1..n where each element moves by one of
 * the displacements {-6,-5,-4,-3,-1,0,2}.  The algorithm
 * keeps a sliding window of the nine possible source positions
 * for the current target index, encoded in a 9‑bit mask.
 * Only the seven positions whose offsets are allowed can be
 * chosen.  At each step the chosen element is removed from the
 * mask, the window is shifted one step to the right, and a new
 * bit is added for the new source position that appears.
 * The DP runs in O(n·512·7) time and uses only 512 states.
 *
 * No hard‑coded values are used, so the function computes any
 * term a(n) (n ≥ 1) exactly, provided a(n) fits in the type seq_t.
 */

#include <stdint.h>

enum { WINDOW = 9, STATES = 1 << WINDOW };          /* 512 states */

/* Positions in the window that are allowed by the displacement set.
 * Window indices: 0 1 2 3 4 5 6 7 8  (offsets: -2,-1,0,1,2,3,4,5,6)
 * Allowed offsets:  -2, 0, 1, 3, 4, 5, 6   →  indices 0,2,3,5,6,7,8
 */
static const int allowed[7] = { 0, 2, 3, 5, 6, 7, 8 };

seq_t a(idx_t n)
{
    if (n == 0)    /* sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    seq_t dp[STATES] = { 0 };
    seq_t next[STATES] = { 0 };

    /* ----  initial mask at position i = 1  ---- */
    int initMask = 0;
    for (int t = 0; t < WINDOW; ++t) {
        /* offset = t - 2  →  source index j = 1 + (t - 2) = t - 1 */
        int j = t - 1;
        if (j < 1 || j > (int)n)
            initMask |= 1 << t;
    }
    dp[initMask] = 1;

    seq_t answer = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* reset the next array for the new step */
        for (int s = 0; s < STATES; ++s)
            next[s] = 0;

        for (int mask = 0; mask < STATES; ++mask) {
            seq_t cnt = dp[mask];
            if (cnt == 0) continue;

            for (int k = 0; k < 7; ++k) {
                int pos = allowed[k];
                if ( (mask & (1 << pos)) == 0 ) {           /* position free */
                    int fullMask = mask | (1 << pos);

                    if (i == n) {
                        answer += cnt;                     /* final assignment */
                    } else {
                        /* shift window right by one:
                           drop the leftmost bit (offset -2)
                           new bit (offset 6) comes from j = i+7
                        */
                        int nextMask = (fullMask >> 1) |
                                       ( (i + 7 <= n) ? 0 : (1 << (WINDOW - 1)) );
                        next[nextMask] += cnt;
                    }
                }
            }
        }

        /* move to the next position */
        for (int s = 0; s < STATES; ++s)
            dp[s] = next[s];
    }

    return answer;
}
