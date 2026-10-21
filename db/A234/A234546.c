/*
 * A234546  Number of (n+1) X (4+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 20, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A234546 – Number of (n+1)x5 arrays with 0..5 entries such that every 2x2
    subblock has its diagonal sum differing from its antidiagonal sum by 3.
    The program enumerates all possible row‑difference states d∈ℤ⁴,
    weights each state by the number of rows that realise it,
    builds a graph where an edge exists iff each component changes by ±3,
    then counts walks of length n in this weighted graph by dynamic
    programming.  The number of states is ≤7776, each has at most 16
    successors, so the algorithm runs in O(S·n) time (S≈200–300) and
    O(S) memory.  It works for any seq_t/idx_t combination.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* The function to be implemented by the grader */
seq_t a(idx_t n)
{
    /* ---------- lazy static initialization ---------- */
    static bool initialized = false;
    /* mapping from difference tuple to state index : -1 means absent */
    static int diff_map[11*11*11*11];
    /* state data (maximum possible is 6^5 = 7776) */
    static int state_d[7776][4];
    static seq_t state_w[7776];
    static size_t state_cnt = 0;
    /* adjacency lists: succ_cnt[i] ≤ 16 */
    static size_t succ_cnt[7776];
    static int succ_idx[7776][16];

    if (!initialized) {
        const int OFF = 5;                /* shift to make differences 0..10 */
        const int DIM = 11;               /* number of possible values per diff */

        /* initialise map */
        for (int i = 0; i < DIM*DIM*DIM*DIM; ++i) diff_map[i] = -1;

        /* enumerate all 6^5 rows */
        int a[5];
        for (a[0] = 0; a[0] <= 5; ++a[0])
        for (a[1] = 0; a[1] <= 5; ++a[1])
        for (a[2] = 0; a[2] <= 5; ++a[2])
        for (a[3] = 0; a[3] <= 5; ++a[3])
        for (a[4] = 0; a[4] <= 5; ++a[4]) {
            int d0 = a[1]-a[0];
            int d1 = a[2]-a[1];
            int d2 = a[3]-a[2];
            int d3 = a[4]-a[3];
            int key = ((((d0+OFF)*DIM + (d1+OFF))*DIM + (d2+OFF))*DIM + (d3+OFF));
            int idx = diff_map[key];
            if (idx == -1) {
                idx = state_cnt++;
                diff_map[key] = idx;
                state_d[idx][0] = d0;
                state_d[idx][1] = d1;
                state_d[idx][2] = d2;
                state_d[idx][3] = d3;
                state_w[idx] = 0;
            }
            state_w[idx] += 1;
        }

        /* build adjacency: each component can change by ±3 independently */
        for (size_t i = 0; i < state_cnt; ++i) {
            succ_cnt[i] = 0;
            for (int mask = 0; mask < 16; ++mask) {
                int nd[4];
                bool ok = true;
                for (int k = 0; k < 4; ++k) {
                    int sign = (mask >> k) & 1 ? 3 : -3;
                    nd[k] = state_d[i][k] + sign;
                    if (nd[k] < -5 || nd[k] > 5) { ok = false; break; }
                }
                if (!ok) continue;
                int key = ((((nd[0]+OFF)*DIM + (nd[1]+OFF))*DIM + (nd[2]+OFF))*DIM + (nd[3]+OFF));
                int j = diff_map[key];
                if (j != -1) {
                    succ_idx[i][succ_cnt[i]++] = j;
                }
            }
        }
        initialized = true;
    }

    /* ---------- dynamic programming for n transitions ---------- */
    if (n == 0) {
        /* not used for the series (offset 1), but keep correct behaviour */
        seq_t sum = 0;
        for (size_t i = 0; i < state_cnt; ++i) sum += state_w[i];
        return sum;
    }

    seq_t *cur = (seq_t*)calloc(state_cnt, sizeof(seq_t));
    seq_t *nxt = (seq_t*)calloc(state_cnt, sizeof(seq_t));
    if (!cur || !nxt) { return 0; } /* malloc failure – unlikely */

    /* initial step: one row */
    for (size_t i = 0; i < state_cnt; ++i) cur[i] = state_w[i];

    for (idx_t step = 0; step < n; ++step) {
        for (size_t i = 0; i < state_cnt; ++i) nxt[i] = 0;

        for (size_t i = 0; i < state_cnt; ++i) {
            seq_t ci = cur[i];
            if (ci == 0) continue;
            for (size_t s = 0; s < succ_cnt[i]; ++s) {
                size_t j = succ_idx[i][s];
                nxt[j] += ci * state_w[j];
            }
        }
        /* swap current and next */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* result is sum of all states after n steps */
    seq_t result = 0;
    for (size_t i = 0; i < state_cnt; ++i) result += cur[i];

    free(cur);
    free(nxt);
    return result;
}
