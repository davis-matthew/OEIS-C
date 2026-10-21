/*
 * A224206  Number of 4Xn 0..3 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 31, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by transfer-matrix DP over columns. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* State encoding:
   each row uses 3 bits: high 2 bits = value 0..3, low 1 bit = flag
   flag = 0 → still in increasing phase; flag = 1 → decreasing phase started.
   State is a 12‑bit number (4096 states). */

static uint8_t col_vals[256][4];          /* column value lookup */
static uint16_t edges[4096 * 256];        /* adjacency list (max 4096*256 edges) */
static int     trans_start[4096];
static int     trans_len[4096];
static int     edge_count;

/* Build transition tables once. */
static void build_transitions(void)
{
    static int built = 0;
    if (built) return;
    built = 1;

    /* Pre‑compute column value table. */
    for (int col = 0; col < 256; ++col) {
        for (int i = 0; i < 4; ++i)
            col_vals[col][i] = (col >> ((3 - i) * 2)) & 3;
    }

    edge_count = 0;
    for (int state = 0; state < 4096; ++state) {
        trans_start[state] = edge_count;
        trans_len[state] = 0;

        int tmp = state;
        uint8_t val[4], flag[4];
        for (int i = 0; i < 4; ++i) {
            int comp = tmp & 7;   /* lower 3 bits */
            tmp >>= 3;
            val[i]  = (comp >> 1) & 3;
            flag[i] = comp & 1;
        }

        for (int col = 0; col < 256; ++col) {
            /* Antidiagonal nondecreasing: val[i] <= col_vals[col][i+1] (i=0..2) */
            if (val[0] > col_vals[col][1]) continue;
            if (val[1] > col_vals[col][2]) continue;
            if (val[2] > col_vals[col][3]) continue;

            uint8_t nf[4];
            int ok = 1;
            for (int i = 0; i < 4; ++i) {
                uint8_t next_val = col_vals[col][i];
                if (flag[i]) {
                    /* Already decreasing: cannot increase. */
                    if (next_val > val[i]) { ok = 0; break; }
                    nf[i] = 1;
                } else {
                    /* Still increasing: allowed to stay or increase; 
                       a decrease switches to decreasing (flag = 1). */
                    if (next_val < val[i]) nf[i] = 1;
                    else nf[i] = 0;
                }
            }
            if (!ok) continue;

            int next_state = 0;
            int mult = 1;
            for (int i = 0; i < 4; ++i) {
                int comp = (col_vals[col][i] << 1) | nf[i];
                next_state += comp * mult;
                mult <<= 3;
            }
            edges[edge_count++] = (uint16_t)next_state;
            trans_len[state] += 1;
        }
    }
}

static seq_t dp_prev[4096];
static seq_t dp_next[4096];

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – not used */

    build_transitions();

    /* Initialise dp_prev for the first column. */
    memset(dp_prev, 0, sizeof(dp_prev));
    for (int col = 0; col < 256; ++col) {
        int state = 0;
        int mult = 1;
        for (int i = 0; i < 4; ++i) {
            int comp = col_vals[col][i] << 1; /* flag 0 */
            state += comp * mult;
            mult <<= 3;
        }
        dp_prev[state] = 1;
    }

    /* Process remaining columns. */
    for (idx_t step = 1; step < n; ++step) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int s = 0; s < 4096; ++s) {
            seq_t val = dp_prev[s];
            if (!val) continue;
            int start = trans_start[s];
            int len   = trans_len[s];
            for (int k = start; k < start + len; ++k) {
                dp_next[edges[k]] += val;
            }
        }
        /* swap dp arrays */
        memcpy(dp_prev, dp_next, sizeof(dp_prev));
    }

    /* Sum all states to obtain the answer. */
    seq_t result = 0;
    for (int s = 0; s < 4096; ++s)
        result += dp_prev[s];
    return result;
}
