/*
 * A223213  3X3X3 triangular graph coloring a rectangular array: number of nX3
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,2
 *          1,2 1,3 1,4 2,4 3,4 2,5 4,5 and every array movement to a
 *          horizontal or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   dynamic programming over 3‑column rows. For an n×3 array over
 *            {0,…,5} where adjacent cells must be connected by an edge of the
 *            graph with edges 0-1,0-2,1-2,1-3,1-4,2-4,3-4,2-5,4-5, a(n)
 *            counts all such arrays. The algorithm enumerates all
 *            horizontally valid row patterns (60 of them), precomputes...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>   /* size_t */

static const bool adj[6][6] = {
    /*0*/ {0,1,1,0,0,0},
    /*1*/ {1,0,1,1,1,0},
    /*2*/ {1,1,0,0,1,1},
    /*3*/ {0,1,0,0,1,0},
    /*4*/ {0,1,1,1,0,1},
    /*5*/ {0,0,1,0,1,0}
};

enum { MAX_STATES = 60 };   /* compile‑time constant */

/* Return the n-th term of OEIS A223213.  index n >= 1.
   The function works for any seq_t/idx_t pair provided by the build system. */
seq_t a(idx_t n)
{
    /* Enumerate all horizontally valid 3‑cell states. */
    struct { int c[3]; } states[MAX_STATES];
    int state_cnt = 0;

    for (int c0 = 0; c0 < 6; ++c0) {
        for (int c1 = 0; c1 < 6; ++c1) {
            if (!adj[c0][c1]) continue;
            for (int c2 = 0; c2 < 6; ++c2) {
                if (!adj[c1][c2]) continue;
                states[state_cnt].c[0] = c0;
                states[state_cnt].c[1] = c1;
                states[state_cnt].c[2] = c2;
                ++state_cnt;
            }
        }
    }

    /* Precompute vertical compatibility between states. */
    static unsigned char compat[MAX_STATES][MAX_STATES];
    for (int i = 0; i < state_cnt; ++i) {
        for (int j = 0; j < state_cnt; ++j) {
            bool ok = true;
            for (int k = 0; k < 3; ++k) {
                if (!adj[ states[i].c[k] ][ states[j].c[k] ]) {
                    ok = false; break;
                }
            }
            compat[i][j] = ok ? 1 : 0;
        }
    }

    /* Dynamic programming over the rows. */
    seq_t dp_prev[MAX_STATES];
    seq_t dp_cur[MAX_STATES];

    /* first row: any state is allowed */
    for (int i = 0; i < state_cnt; ++i) dp_prev[i] = 1;

    /* process subsequent rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < state_cnt; ++i) dp_cur[i] = 0;

        for (int t = 0; t < state_cnt; ++t) {
            seq_t val = dp_prev[t];
            if (val == 0) continue;
            for (int s = 0; s < state_cnt; ++s) {
                if (compat[t][s]) dp_cur[s] += val;
            }
        }

        for (int i = 0; i < state_cnt; ++i) dp_prev[i] = dp_cur[i];
    }

    /* total number of arrays = sum over last row states */
    seq_t ans = 0;
    for (int i = 0; i < state_cnt; ++i) ans += dp_prev[i];
    return ans;
}
