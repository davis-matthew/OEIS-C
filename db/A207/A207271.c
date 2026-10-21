/*
 * A207271  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays avoiding the patterns * 000
 *            and 011 horizontally and the patterns 011 and 101 vertically. *
 *            Computed by dynamic programming over admissible columns
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 26, u64 n <= 63, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* admissible 4‑bit columns satisfying the vertical constraints */
static const unsigned char VALID_COLS[9] = {0,1,2,4,8,9,12,14,15};
static const size_t V = 9; /* number of admissible columns */

seq_t a(idx_t n)
{
    /* a(0) is undefined in OEIS; return 0 for safety. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)V;           /* 9 valid columns */
    if (n == 2) return (seq_t)V * (seq_t)V;/* pairs of columns */

    size_t M = V * V;          /* number of states (previous two columns) */

    seq_t *dp  = (seq_t*)calloc(M, sizeof(seq_t));
    seq_t *tmp = (seq_t*)calloc(M, sizeof(seq_t));
    if (!dp || !tmp) {
        free(dp);
        free(tmp);
        return (seq_t)0;
    }

    /* initial states: one way for every admissible column pair */
    for (size_t i = 0; i < M; ++i) dp[i] = (seq_t)1;

    /* extend the array column by column */
    for (idx_t step = 3; step <= n; ++step) {
        for (size_t i = 0; i < M; ++i) {
            seq_t cur = dp[i];
            if (cur == (seq_t)0) continue;

            unsigned int prev2Idx = (unsigned int)(i / V);
            unsigned int prev1Idx = (unsigned int)(i % V);
            unsigned int prev2Col = VALID_COLS[prev2Idx];
            unsigned int prev1Col = VALID_COLS[prev1Idx];

            /* try every admissible next column */
            for (unsigned int k = 0; k < V; ++k) {
                unsigned int newCol = VALID_COLS[k];
                int ok = 1;

                /* check horizontal constraints for each of the 4 rows */
                for (int r = 0; r < 4; ++r) {
                    int aBit = (prev2Col >> r) & 1;
                    int bBit = (prev1Col >> r) & 1;
                    int cBit = (newCol >> r) & 1;
                    if ((aBit == 0 && bBit == 0 && cBit == 0) ||
                        (aBit == 0 && bBit == 1 && cBit == 1)) {
                        ok = 0;
                        break;
                    }
                }
                if (!ok) continue;

                size_t newState = prev1Idx * V + k;
                tmp[newState] += cur;
            }
        }

        /* swap buffers and clear the temporary buffer for the next step */
        seq_t *swap = dp;
        dp = tmp;
        tmp = swap;
        memset(tmp, 0, M * sizeof(seq_t));
    }

    /* sum all ending states to get the total number of arrays */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < M; ++i) result += dp[i];

    free(dp);
    free(tmp);
    return result;
}
