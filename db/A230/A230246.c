/*
 * A230246  Number of nX3 0..3 arrays x(i,j) with each element horizontally or
 *          vertically next to at least one element with value (x(i,j)+1) mod
 *          4, and upper left element zero.
 * https://oeis.org/A230246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements dynamic‑programming for OEIS sequence A230246 */
/* A(n) = number of n×3 arrays of {0,1,2,3} with the
   “next‑value” neighbour condition and upper‑left entry 0. */
/* Uses a row‑by‑row DP storing for each row a 3‑bit mask
   indicating whether each cell already meets the condition
   via its upper and horizontal neighbours.  The mask for the
   previous row is checked when the next row is added. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Precompute static tables once */
    static bool precomputed = false;
    static uint8_t row_val[64][3];
    static uint8_t reqmask[64][64];     /* bits that must be set in previous mask */
    static uint8_t nextmask[64][64];    /* mask for the new row */
    static uint8_t no_up_mask[64];      /* mask when there is no upper neighbour */

    if (!precomputed) {
        for (idx_t r = 0; r < 64; ++r) {
            row_val[r][0] = (uint8_t)(r & 3);
            row_val[r][1] = (uint8_t)((r >> 2) & 3);
            row_val[r][2] = (uint8_t)((r >> 4) & 3);

            /* mask without an upper neighbour (used for the first row) */
            uint8_t mask = 0;
            for (int j = 0; j < 3; ++j) {
                uint8_t target = (row_val[r][j] + 1) & 3;
                bool sat = false;
                if (j > 0 && row_val[r][j-1] == target) sat = true;
                if (j < 2 && row_val[r][j+1] == target) sat = true;
                if (sat) mask |= (1U << j);
            }
            no_up_mask[r] = mask;
        }

        for (idx_t prev = 0; prev < 64; ++prev) {
            for (idx_t cur = 0; cur < 64; ++cur) {
                /* Requirement mask: bits where cur cell is NOT the
                   next value of prev cell – those bits must already be
                   set in prev’s mask. */
                uint8_t require = 0;
                for (int j = 0; j < 3; ++j) {
                    uint8_t target_prev = (row_val[prev][j] + 1) & 3;
                    if (row_val[cur][j] != target_prev)
                        require |= (1U << j);
                }
                reqmask[prev][cur] = require;

                /* mask for the current row considering its upper neighbour */
                uint8_t curmask = 0;
                for (int j = 0; j < 3; ++j) {
                    uint8_t target_cur = (row_val[cur][j] + 1) & 3;
                    bool sat = false;
                    if (j > 0 && row_val[cur][j-1] == target_cur) sat = true;
                    if (j < 2 && row_val[cur][j+1] == target_cur) sat = true;
                    if (row_val[prev][j] == target_cur) sat = true;
                    if (sat) curmask |= (1U << j);
                }
                nextmask[prev][cur] = curmask;
            }
        }
        precomputed = true;
    }

    /* DP arrays: index = row*8 + mask (mask 0..7) */
    static seq_t dp[512];
    static seq_t newdp[512];

    /* Initialize for the first row (upper‑left fixed to 0) */
    for (idx_t i = 0; i < 512; ++i) dp[i] = 0;
    for (idx_t row = 0; row < 64; ++row) {
        if (row_val[row][0] != 0) continue;          /* upper‑left must be 0 */
        idx_t idx = (row << 3) | no_up_mask[row];
        dp[idx] += 1;
    }

    /* Process subsequent rows */
    for (idx_t step = 1; step < n; ++step) {
        for (idx_t i = 0; i < 512; ++i) newdp[i] = 0;
        for (idx_t prev = 0; prev < 64; ++prev) {
            for (int prevMask = 0; prevMask < 8; ++prevMask) {
                idx_t prevIdx = (prev << 3) | prevMask;
                seq_t count = dp[prevIdx];
                if (count == 0) continue;
                for (idx_t cur = 0; cur < 64; ++cur) {
                    uint8_t require = reqmask[prev][cur];
                    if ( (prevMask & require) != require ) continue; /* invalid */
                    uint8_t curMask = nextmask[prev][cur];
                    idx_t curIdx = (cur << 3) | curMask;
                    newdp[curIdx] += count;
                }
            }
        }
        /* swap dp and newdp */
        for (idx_t i = 0; i < 512; ++i) dp[i] = newdp[i];
    }

    /* Final row must have all bits set (mask 7) */
    seq_t result = 0;
    for (idx_t row = 0; row < 64; ++row) {
        result += dp[(row << 3) | 7];
    }
    return result;
}
