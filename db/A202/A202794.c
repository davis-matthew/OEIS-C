/*
 * A202794  Number of nX7 binary arrays with every one adjacent to another one
 *          horizontally, diagonally, antidiagonally or vertically.
 * https://oeis.org/A202794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays where each 1 is adjacent
 *            horizontally, vertically, or diagonally to at least one other 1.
 *            The algorithm uses dynamic programming over all possible rows
 *            and the unsatisfied‑ones that may be resolved by the next row
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* There are 3^7 = 2187 possible (row,unsatisfied‑set) states.
       Buffer size is therefore a little larger than this. */
    enum { MAX_STATES = 2200, COLS = 7, ROW_MASKS = 128 };

    static int initialized = 0;
    static int stateCount = 0;
    static int16_t idx_map[ROW_MASKS][ROW_MASKS];     /* mapping (curr,u) -> state index, -1 if invalid */
    static uint8_t curr_of_state[MAX_STATES];
    static uint8_t u_of_state[MAX_STATES];
    static uint8_t unsatMask[ROW_MASKS][ROW_MASKS];   /* unsatisfied bits of curr w.r.t. prev */
    static uint8_t validNext[ROW_MASKS][ROW_MASKS];   /* does next row q satisfy unsatisfied bits u? */

    if (!initialized) {
        /* Build state mapping */
        for (int curr = 0; curr < ROW_MASKS; ++curr) {
            for (int u = 0; u < ROW_MASKS; ++u) {
                if ((u & ~curr) == 0) {            /* u is subset of curr */
                    idx_map[curr][u] = (int16_t)stateCount;
                    curr_of_state[stateCount] = (uint8_t)curr;
                    u_of_state[stateCount] = (uint8_t)u;
                    ++stateCount;
                } else {
                    idx_map[curr][u] = -1;
                }
            }
        }

        /* Precompute unsatisfied mask for each pair (prev,curr) */
        for (int prev = 0; prev < ROW_MASKS; ++prev) {
            for (int curr = 0; curr < ROW_MASKS; ++curr) {
                uint8_t mask = 0;
                for (int j = 0; j < COLS; ++j) {
                    if (curr & (1 << j)) {
                        int hasPrev = 0;
                        for (int k = j - 1; k <= j + 1; ++k)
                            if (k >= 0 && k < COLS && (prev & (1 << k))) {
                                hasPrev = 1; break;
                            }
                        int hasSame = (j > 0 && (curr & (1 << (j - 1)))) ||
                                      (j < COLS - 1 && (curr & (1 << (j + 1))));
                        if (!hasPrev && !hasSame)
                            mask |= (1 << j);
                    }
                }
                unsatMask[prev][curr] = mask;
            }
        }

        /* Precompute validity of the next row for a given set of unsatisfied bits */
        for (int u = 0; u < ROW_MASKS; ++u) {
            for (int q = 0; q < ROW_MASKS; ++q) {
                int ok = 1;
                for (int bit = 0; bit < COLS; ++bit) {
                    if (u & (1 << bit)) {
                        int has = 0;
                        if (bit > 0 && (q & (1 << (bit - 1)))) has = 1;
                        if (q & (1 << bit)) has = 1;
                        if (bit < COLS - 1 && (q & (1 << (bit + 1)))) has = 1;
                        if (!has) { ok = 0; break; }
                    }
                }
                validNext[u][q] = ok;
            }
        }
        initialized = 1;
    }

    /* DP arrays (fixed maximum size) */
    seq_t cur[MAX_STATES];
    seq_t nxt[MAX_STATES];
    for (int i = 0; i < stateCount; ++i) cur[i] = (seq_t)0, nxt[i] = (seq_t)0;

    /* Initial row: previous row is all zeros */
    for (int curr = 0; curr < ROW_MASKS; ++curr) {
        uint8_t u = unsatMask[0][curr];
        int s = idx_map[curr][u];
        cur[s] = (seq_t)1;
    }

    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (int s = 0; s < stateCount; ++s)
            if (u_of_state[s] == 0) ans += cur[s];
        return ans;
    }

    /* Transition over remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (int s = 0; s < stateCount; ++s) nxt[s] = (seq_t)0;
        for (int s = 0; s < stateCount; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;
            uint8_t curr = curr_of_state[s];
            uint8_t u = u_of_state[s];
            for (int q = 0; q < ROW_MASKS; ++q) {
                if (!validNext[u][q]) continue;
                uint8_t new_u = unsatMask[curr][q];
                int t = idx_map[q][new_u];
                nxt[t] += val;
            }
        }
        /* copy nxt into cur for next iteration */
        for (int i = 0; i < stateCount; ++i) cur[i] = nxt[i];
    }

    /* Sum states where the last row has no unsatisfied 1s */
    seq_t ans = (seq_t)0;
    for (int s = 0; s < stateCount; ++s)
        if (u_of_state[s] == 0) ans += cur[s];
    return ans;
}
