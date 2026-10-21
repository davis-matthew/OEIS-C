/*
 * A228659  Number of nX7 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays with top‑left 1 and no two
 *            ones adjacent horizontally, vertically, or diagonally. Computed
 *            by dynamic programming on row patterns
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Enumerate admissible 7‑bit row patterns (no adjacent 1's). */
    static int  states[128];
    static int  state_count = 0;
    /* Neighbor lists for transitions (no diagonal adjacency). */
    static int  neighbors[128][128];
    static int  neighbor_count[128];
    static int  initialized = 0;

    if (!initialized) {
        /* All patterns 0..127 */
        for (int pat = 0; pat < 128; ++pat) {
            int ok = 1;
            for (int col = 0; col < 6; ++col)
                if ((pat & (1 << col)) && (pat & (1 << (col + 1)))) {
                    ok = 0;
                    break;
                }
            if (ok) {
                states[state_count++] = pat;
            }
        }

        /* Build adjacency list: for each pair of admissible patterns,
           allow transition if left/right diagonal bits are not both 1. */
        for (int i = 0; i < state_count; ++i) {
            unsigned int a = (unsigned int)states[i];
            for (int j = 0; j < state_count; ++j) {
                unsigned int b = (unsigned int)states[j];
                if (((a << 1) & b) == 0 && ((a >> 1) & b) == 0) {
                    neighbors[i][neighbor_count[i]++] = j;
                }
            }
        }
        initialized = 1;
    }

    /* Allocate DP arrays for current and next row counts. */
    seq_t *cur  = (seq_t *)malloc(state_count * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(state_count * sizeof(seq_t));
    if (!cur || !next) {
        /* Memory allocation failed: return 0 as a safe fallback. */
        free(cur);
        free(next);
        return (seq_t)0;
    }

    /* Initial counts: first row must have bit 0 = 1 (top‑left 1). */
    for (int i = 0; i < state_count; ++i) {
        cur[i] = 0;
        if (states[i] & 1)  /* least significant bit is column 0 */
            cur[i] = (seq_t)1;
    }

    /* Transition through rows 2..n. */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero next vector. */
        for (int j = 0; j < state_count; ++j)
            next[j] = (seq_t)0;

        /* Transition from each current state. */
        for (int i = 0; i < state_count; ++i) {
            seq_t val = cur[i];
            if (val == (seq_t)0) continue;
            int cnt = neighbor_count[i];
            for (int k = 0; k < cnt; ++k) {
                int j = neighbors[i][k];
                next[j] = next[j] + val;
            }
        }
        /* Swap cur and next for next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum all final states to obtain the answer. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < state_count; ++i)
        result = result + cur[i];

    free(cur);
    free(next);
    return result;
}
