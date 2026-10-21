/*
 * A234119  Number of (n+1)X(5+1) 0..2 arrays with every 2X2 subblock having
 *          the absolute values of all six edge and diagonal differences no
 *          larger than 1.
 * https://oeis.org/A234119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A234119: Number of (n+1)×6 arrays with entries 0..2 such that
 * for every 2×2 subblock all edge and diagonal differences are
 * at most 1 in absolute value.  The function uses a transfer‑matrix
 * DP over the valid row patterns.  States and transitions are
 * precomputed once; each call performs O(n·s·d) additions where
 * s ≤ 240 and d ≤ 50. */

#include <stdint.h>

#define MAX_STATES 729 /* 3^6 */
#define MAX_COLS   6

/* State representation: t[i][c] is the value in column c of state i */
static uint8_t  t[MAX_STATES][MAX_COLS];
static uint16_t neigh_cnt[MAX_STATES];
static uint16_t neigh[MAX_STATES][MAX_STATES];
static int      num_states;

/* Two buffers for DP; pointers will be swapped */
static seq_t dp0[MAX_STATES];
static seq_t dp1[MAX_STATES];
static seq_t *cur  = dp0;
static seq_t *next = dp1;

/* One‑time precomputation of all admissible rows and the transition
 * matrix.  This function is called lazily in seq_t a(idx_t). */
static void init(void)
{
    int idx = 0;
    for (int num = 0; num < 729; ++num) {            /* all 3^6 patterns */
        int v = num;
        for (int c = 0; c < MAX_COLS; ++c) {
            t[idx][c] = (uint8_t)(v % 3);
            v /= 3;
        }
        int ok = 1;
        for (int c = 0; c < MAX_COLS - 1; ++c) {     /* horizontal diffs */
            int d = (int)t[idx][c] - (int)t[idx][c + 1];
            if (d < -1 || d > 1) { ok = 0; break; }
        }
        if (ok) idx++;
    }
    num_states = idx;

    /* Count transitions between all admissible rows. */
    for (int i = 0; i < num_states; ++i) neigh_cnt[i] = 0;
    for (int i = 0; i < num_states; ++i) {
        for (int j = 0; j < num_states; ++j) {
            int k, d;

            /* vertical and first diagonal differences */
            for (k = 0; k < MAX_COLS; ++k) {
                d = (int)t[i][k] - (int)t[j][k];
                if (d < -1 || d > 1) break;
            }
            if (k < MAX_COLS) continue;

            /* second diagonal differences (off‑by‑one columns) */
            for (k = 0; k < MAX_COLS - 1; ++k) {
                d = (int)t[i][k] - (int)t[j][k + 1];
                if (d < -1 || d > 1) break;
                d = (int)t[j][k] - (int)t[i][k + 1];
                if (d < -1 || d > 1) break;
            }
            if (k < MAX_COLS - 1) continue;

            neigh[i][neigh_cnt[i]++] = (uint16_t)j;
        }
    }
}

/* --------------------------------------------------------------- */
/* Public sequence function                                         */

seq_t a(idx_t n)                  /* 1‑based: a(1) = 14527, …         */
{
    static int ready = 0;
    if (!ready) { init(); ready = 1; }

    if (n == 0) return 0;          /* below offset – irrelevant */

    /* initial vector: any first row is allowed (count 1 for each state) */
    for (int i = 0; i < num_states; ++i) cur[i] = 1;

    /* Apply n transitions – each adds a new row. */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < num_states; ++i) next[i] = 0;

        for (int i = 0; i < num_states; ++i) {
            seq_t val = cur[i];
            if (val == 0) continue;
            uint16_t cnt = neigh_cnt[i];
            for (uint16_t k = 0; k < cnt; ++k) {
                int j = neigh[i][k];
                next[j] += val;
            }
        }
        /* swap cur and next for the next iteration */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* Sum all ending states to get the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < num_states; ++i) total += cur[i];
    return total;
}
