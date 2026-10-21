/*
 * A222441  Number of nX5 0..3 arrays with entries increasing mod 4 by 0, 1 or
 *          2 rightwards and downwards, starting with upper left zero.
 * https://oeis.org/A222441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A222441: Number of n×5 0..3 arrays with entries increasing mod 4 by 0, 1 or 2 rightwards and downwards, starting with upper left zero.
   Computed by dynamic programming over all horizontal‑valid row patterns. */

#include <stdbool.h>

static int num_states;
static int id[1024];
static int state_pat[1024];
static int adj[1024][256];
static int adj_cnt[1024];

static void compute_tables(void) {
    static bool done = false;
    if (done) return;
    done = true;

    for (int p = 0; p < 1024; p++) id[p] = -1;

    int st = 0;
    for (int p = 0; p < 1024; p++) {
        bool ok = true;
        for (int j = 0; j < 4; j++) {
            int d1 = (p >> (2 * j)) & 3;
            int d2 = (p >> (2 * (j + 1))) & 3;
            int diff = (d2 - d1 + 4) % 4;
            if (diff == 3) { ok = false; break; }
        }
        if (ok) {
            id[p] = st;
            state_pat[st] = p;
            st++;
        }
    }
    num_states = st;

    for (int i = 0; i < num_states; i++) {
        int pat_i = state_pat[i];
        int cnt = 0;
        for (int j = 0; j < num_states; j++) {
            int pat_j = state_pat[j];
            bool vert_ok = true;
            for (int col = 0; col < 5; col++) {
                int d1 = (pat_i >> (2 * col)) & 3;
                int d2 = (pat_j >> (2 * col)) & 3;
                int diff = (d2 - d1 + 4) % 4;
                if (diff == 3) { vert_ok = false; break; }
            }
            if (vert_ok) adj[i][cnt++] = j;
        }
        adj_cnt[i] = cnt;
    }
}

seq_t a(idx_t n) {
    compute_tables();
    if (n == 0) return 0;

    seq_t dp_cur[1024];
    seq_t dp_next[1024];

    for (int i = 0; i < num_states; i++) dp_cur[i] = (seq_t)0;
    for (int i = 0; i < num_states; i++) {
        int p = state_pat[i];
        if ((p & 3) == 0) dp_cur[i] = (seq_t)1;
    }

    if (n == 1) {
        seq_t ans = (seq_t)0;
        for (int i = 0; i < num_states; i++) ans += dp_cur[i];
        return ans;
    }

    for (idx_t row = 2; row <= n; row++) {
        for (int i = 0; i < num_states; i++) dp_next[i] = (seq_t)0;
        for (int i = 0; i < num_states; i++) {
            if (dp_cur[i] == (seq_t)0) continue;
            seq_t val = dp_cur[i];
            int *neigh = adj[i];
            int c = adj_cnt[i];
            for (int k = 0; k < c; k++) {
                int j = neigh[k];
                dp_next[j] += val;
            }
        }
        for (int i = 0; i < num_states; i++) dp_cur[i] = dp_next[i];
    }

    seq_t ans = (seq_t)0;
    for (int i = 0; i < num_states; i++) ans += dp_cur[i];
    return ans;
}
