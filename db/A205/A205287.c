/*
 * A205287  Number of (n+1) X 3 0..2 arrays with permanents of 2 X 2 subblocks
 *          differing from horizontal and vertical neighbor permanents.
 * https://oeis.org/A205287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of (n+1)×3 0..2 arrays with permanents of 2×2 subblocks
 *            differing from horizontal and vertical neighbour permanents.
 *            Computed by dynamic programming over row transitions
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

extern seq_t add_seq_t(seq_t a, seq_t b); /* if needed for arbitrary‑precision */

// Helper arrays with maximum possible size
#define MAX_ROWS   27          // 3‑digit base‑3 numbers
#define MAX_PERM   9           // permanents 0..8
#define MAX_STATES 2000        // conservative upper bound
#define MAX_EDGES  27

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* ---------- static data initialised once ---------- */
    static bool initialized = false;
    static int state_id[MAX_ROWS][MAX_PERM][MAX_PERM];
    static int row_of_state[MAX_STATES];
    static int l_of_state[MAX_STATES];
    static int r_of_state[MAX_STATES];
    static int edge_cnt[MAX_STATES];
    static int edges[MAX_STATES][MAX_EDGES];
    static uint8_t row_dig[MAX_ROWS][3];
    static int perm_left_val[MAX_ROWS][MAX_ROWS];
    static int perm_right_val[MAX_ROWS][MAX_ROWS];
    static int total_states = 0;

    if (!initialized) {
        /* compute row digits */
        for (int r = 0; r < MAX_ROWS; ++r) {
            row_dig[r][0] = r % 3;
            row_dig[r][1] = (r / 3) % 3;
            row_dig[r][2] = r / 9;
        }

        /* precompute permanents for every pair of rows */
        for (int i = 0; i < MAX_ROWS; ++i)
            for (int j = 0; j < MAX_ROWS; ++j) {
                int a0 = row_dig[i][0], a1 = row_dig[i][1], a2 = row_dig[i][2];
                int b0 = row_dig[j][0], b1 = row_dig[j][1], b2 = row_dig[j][2];
                perm_left_val[i][j]  = a0 * b1 + a1 * b0;
                perm_right_val[i][j] = a1 * b2 + a2 * b1;
            }

        /* initialise state table with -1 */
        for (int i = 0; i < MAX_ROWS; ++i)
            for (int l = 0; l < MAX_PERM; ++l)
                for (int r = 0; r < MAX_PERM; ++r)
                    state_id[i][l][r] = -1;

        /* enumerate all valid states (row, left_perm, right_perm) */
        for (int prev = 0; prev < MAX_ROWS; ++prev)
            for (int curr = 0; curr < MAX_ROWS; ++curr) {
                int lp = perm_left_val[prev][curr];
                int rp = perm_right_val[prev][curr];
                if (lp == rp) continue;                 /* horizontal equality forbidden */
                if (state_id[curr][lp][rp] == -1) {
                    state_id[curr][lp][rp] = total_states;
                    row_of_state[total_states] = curr;
                    l_of_state[total_states]  = lp;
                    r_of_state[total_states]  = rp;
                    ++total_states;
                }
            }

        /* build directed edges between states */
        for (int s = 0; s < total_states; ++s) {
            int curr = row_of_state[s];
            int lp   = l_of_state[s];
            int rp   = r_of_state[s];
            int cnt = 0;
            for (int nxt = 0; nxt < MAX_ROWS; ++nxt) {
                int lnp = perm_left_val[curr][nxt];
                int rnp = perm_right_val[curr][nxt];
                if (lnp == rnp) continue;           /* horizontal equality at new block */
                if (lp == lnp) continue;            /* vertical equality on left channel */
                if (rp == rnp) continue;            /* vertical equality on right channel */
                int tgt = state_id[nxt][lnp][rnp];
                if (tgt == -1) continue;           /* should not happen */
                edges[s][cnt++] = tgt;
            }
            edge_cnt[s] = cnt;
        }
        initialized = true;
    }

    /* ---------- dynamic programming for the given n ---------- */
    static seq_t dp_cur[MAX_STATES];
    static seq_t dp_next[MAX_STATES];
    for (int i = 0; i < total_states; ++i) dp_cur[i] = 0;

    /* initial two rows */
    for (int p = 0; p < MAX_ROWS; ++p)
        for (int c = 0; c < MAX_ROWS; ++c) {
            int lp = perm_left_val[p][c];
            int rp = perm_right_val[p][c];
            if (lp == rp) continue;
            int id = state_id[c][lp][rp];
            dp_cur[id] += (seq_t)1;
        }

    if (n == 1) {
        seq_t ans = 0;
        for (int i = 0; i < total_states; ++i) ans += dp_cur[i];
        return ans;
    }

    idx_t steps = n - 1;   /* number of additional rows to add */
    for (idx_t step = 0; step < steps; ++step) {
        for (int i = 0; i < total_states; ++i) dp_next[i] = 0;
        for (int s = 0; s < total_states; ++s) {
            if (dp_cur[s] == 0) continue;
            seq_t val = dp_cur[s];
            int ec = edge_cnt[s];
            for (int t = 0; t < ec; ++t) {
                int tgt = edges[s][t];
                dp_next[tgt] += val;
            }
        }
        /* copy back to dp_cur */
        for (int i = 0; i < total_states; ++i) dp_cur[i] = dp_next[i];
    }

    seq_t ans = 0;
    for (int i = 0; i < total_states; ++i) ans += dp_cur[i];
    return ans;
}
