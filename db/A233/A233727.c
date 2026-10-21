/*
 * A233727  Number of (n+1)X(2+1) 0..3 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 6
 *          and no adjacent elements equal.
 * https://oeis.org/A233727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×3 arrays over {0,1,2,3} with no two
 *            adjacent equal entries and with every 2×2 subblock satisfying
 *            |a–b|+|a–c|+|b–d|+|c–d| = 6. The function constructs a 36‑state
 *            automaton (one state per admissible row) and counts length‑(n+1)
 *            walks in that automaton
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed by the runtime */
#include <stddef.h>
#include <stdint.h>

/* Helper: absolute difference of two small ints (0..3) */
static inline int abs_diff(int a, int b)
{
    int d = a - b;
    return d < 0 ? -d : d;
}

/* The main sequence function */
seq_t a(idx_t n)
{
    /* Static data: enumeration of all admissible rows and their
       directed transitions.  These are computed once. */
    static int init = 0;
    static uint8_t state[36][3];          /* all valid rows */
    static int neigh[36][36];             /* adjacency list */
    static int neigh_cnt[36];             /* number of outgoing edges per row */

    /* Build state set and transitions on first call */
    if (!init) {
        int idx = 0;
        for (int v1 = 0; v1 < 4; ++v1) {
            for (int v2 = 0; v2 < 4; ++v2) {
                if (v1 == v2) continue;
                for (int v3 = 0; v3 < 4; ++v3) {
                    if (v2 == v3) continue;
                    state[idx][0] = (uint8_t)v1;
                    state[idx][1] = (uint8_t)v2;
                    state[idx][2] = (uint8_t)v3;
                    ++idx;
                }
            }
        }
        /* Sanity check: should be 36 rows */
        /* Compute transitions */
        for (int i = 0; i < 36; ++i) {
            neigh_cnt[i] = 0;
            for (int j = 0; j < 36; ++j) {
                /* vertical adjacency constraint */
                int ok = 1;
                for (int k = 0; k < 3; ++k) {
                    if (state[i][k] == state[j][k]) { ok = 0; break; }
                }
                if (!ok) continue;
                /* 2×2 block constraints for columns 0-1 and 1-2 */
                for (int col = 0; col < 2; ++col) {
                    int top    = abs_diff(state[i][col],     state[i][col+1]);
                    int left   = abs_diff(state[i][col],     state[j][col]);
                    int right  = abs_diff(state[i][col+1],   state[j][col+1]);
                    int bottom = abs_diff(state[j][col],     state[j][col+1]);
                    if (top + left + right + bottom != 6) { ok = 0; break; }
                }
                if (ok) {
                    neigh[i][neigh_cnt[i]++] = j;
                }
            }
        }
        init = 1;
    }

    /* Dynamic programming: count walks of length n (n transitions) */
    seq_t cur[36];
    seq_t next[36];
    /* initial state: one row, each state appears once */
    for (int i = 0; i < 36; ++i) cur[i] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        /* zero next counts */
        for (int i = 0; i < 36; ++i) next[i] = 0;
        /* accumulate transitions */
        for (int from = 0; from < 36; ++from) {
            seq_t val = cur[from];
            if (val == 0) continue;
            for (int e = 0; e < neigh_cnt[from]; ++e) {
                int to = neigh[from][e];
                next[to] += val;
            }
        }
        if (step == n) {
            /* sum of all endpoints gives a(n) */
            seq_t total = 0;
            for (int i = 0; i < 36; ++i) total += next[i];
            return total;
        }
        /* prepare for next iteration */
        for (int i = 0; i < 36; ++i) cur[i] = next[i];
    }
    /* Should never reach here */
    return 0;
}
