/*
 * A234187  Number of (n+1) X (3+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A234187
 * Formula: a(n) = number of (n+1)×4 arrays with entries in 0..6 such that
 * every 2×2 subblock has its diagonal sum differing from its antidiagonal
 * sum by 2 (i.e. the difference of the two sums has absolute value 2). */

/* This implementation uses a transfer‑matrix DP on the difference vector
 * (d0,d1,d2) between successive columns of a row.  The array of four values
 * in a row determines its “width”; a row can start with any base value as
 * long as all four entries lie in 0..6.  The number of possible base values
 * is 7‑(maximum‑minimum).  Two consecutive rows are compatible iff the
 * difference between each pair of corresponding columns changes by ±2.
 * The number of arrays of height h = n+1 is obtained by n DP steps on
 * this small graph.

 * Complexity: O(num_states + n·num_states), where num_states ≤2197.
 *
 * The code is fully portable C‑11 and C++17, uses only integer arithmetics
 * and the utility back‑end, and is memoised for repeated calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    /* static data ------------------------------------------------------------ */
    static bool init = false;
    static int  num_states = 0;
    static int  state_d[2197][3];
    static seq_t weight_state[2197];
    static unsigned char out_deg[2197];
    static int  out_adj[2197][8];
    static int  idx_map[2197];   /* map from coded triple to state id   */

    if (!init) {
        /* initialise mapping and state data */
        for (int i = 0; i < 2197; ++i) idx_map[i] = -1;
        num_states = 0;
        for (int d0 = -6; d0 <= 6; ++d0)
            for (int d1 = -6; d1 <= 6; ++d1)
                for (int d2 = -6; d2 <= 6; ++d2) {
                    int p[4];
                    p[0] = 0;
                    p[1] = d0;
                    p[2] = d0 + d1;
                    p[3] = d0 + d1 + d2;
                    int mn = p[0], mx = p[0];
                    for (int j = 1; j < 4; ++j) {
                        if (p[j] < mn) mn = p[j];
                        if (p[j] > mx) mx = p[j];
                    }
                    int width = mx - mn;
                    if (width <= 6) {
                        int id = num_states;
                        int code = (d0 + 6) * 13 * 13 + (d1 + 6) * 13 + (d2 + 6);
                        idx_map[code] = id;
                        state_d[id][0] = d0;
                        state_d[id][1] = d1;
                        state_d[id][2] = d2;
                        weight_state[id] = (seq_t)(7 - width);
                        ++num_states;
                    }
                }

        /* build adjacency: each difference may change by +2 or –2 independently */
        for (int i = 0; i < num_states; ++i) {
            out_deg[i] = 0;
            for (int mask = 0; mask < 8; ++mask) {
                int nd[3];
                for (int k = 0; k < 3; ++k) {
                    int inc = (mask >> k) & 1 ? 2 : -2;
                    nd[k] = state_d[i][k] + inc;
                }
                if (nd[0] < -6 || nd[0] > 6 ||
                    nd[1] < -6 || nd[1] > 6 ||
                    nd[2] < -6 || nd[2] > 6)
                    continue;  /* invalid state, skip */

                int code = (nd[0] + 6) * 13 * 13 + (nd[1] + 6) * 13 + (nd[2] + 6);
                int j = idx_map[code];
                if (j >= 0) {
                    out_adj[i][out_deg[i]++] = j;
                }
            }
        }

        init = true;
    }

    /* the sequence is defined for n ≥ 1; return 0 for n = 0 as a safe default */
    if (n == 0) return 0;

    /* DP arrays -------------------------------------------------------------- */
    seq_t *cnt      = alloc_seq_array((size_t)num_states);
    seq_t *cnt_next = alloc_seq_array((size_t)num_states);

    /* initial row: choose any first row, weight is number of possible base values */
    for (int i = 0; i < num_states; ++i) cnt[i] = weight_state[i];

    /* perform n transitions (array has n+1 rows) */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < num_states; ++i) cnt_next[i] = 0;
        for (int i = 0; i < num_states; ++i) {
            if (cnt[i] == 0) continue;
            seq_t val = cnt[i];
            for (int e = 0; e < out_deg[i]; ++e) {
                int j = out_adj[i][e];
                cnt_next[j] += val * weight_state[j];
            }
        }
        seq_t *tmp = cnt; cnt = cnt_next; cnt_next = tmp;
    }

    /* sum of counts over all ending states gives the answer */
    seq_t result = 0;
    for (int i = 0; i < num_states; ++i) result += cnt[i];

    free_seq_array(cnt);
    free_seq_array(cnt_next);
    return result;
}
