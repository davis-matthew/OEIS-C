/*
 * A209227  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A209227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209227: Number of 6×n binary matrices avoiding
   horizontal subpatterns 000 and 010, and vertical subpatterns 010 and 111.
   The implementation uses dynamic programming on column patterns.
   Initial values: a(1)=19, a(2)=361. */
#include <stdint.h>
#include <stdbool.h>

#define MAX_M 19                    /* number of valid column patterns */
#define MAX_STATES (MAX_M*MAX_M)    /* M×M states for two consecutive columns */

static bool   col_valid[64] = {0};
static int    col_pat[MAX_M];
static int    col_cnt = 0;          /* number of valid columns */

static unsigned char next_len[MAX_STATES];
static int           next_idx[MAX_STATES][MAX_M];

static bool trans_init = false;

/* Build the tables of valid column patterns and transition graph */
static void init_tables(void)
{
    if (trans_init) return;

    /* 1. Compute valid column patterns (vertical constraints) */
    int m = 0;
    for (int p = 0; p < 64; ++p) {
        bool ok = true;
        for (int r = 0; r <= 3 && ok; ++r) {
            int a = (p >> r)     & 1;
            int b = (p >> (r+1)) & 1;
            int c = (p >> (r+2)) & 1;
            /* forbid vertical 010 and 111 */
            if ((a==0 && b==1 && c==0) || (a==1 && b==1 && c==1))
                ok = false;
        }
        if (ok) {
            col_pat[m++] = p;
            col_valid[p] = true;
        }
    }
    col_cnt = m;        /* should be 19 */

    /* 2. Build transition table between pairs of columns (horizontal constraints) */
    for (int i = 0; i < col_cnt; ++i) {
        int pat_a = col_pat[i];
        for (int j = 0; j < col_cnt; ++j) {
            int pat_b = col_pat[j];
            int idx   = i * col_cnt + j;
            next_len[idx] = 0;
            for (int k = 0; k < col_cnt; ++k) {
                int pat_c = col_pat[k];
                bool ok = true;
                for (int r = 0; r < 6 && ok; ++r) {
                    int a = (pat_a >> r) & 1;
                    int b = (pat_b >> r) & 1;
                    int c = (pat_c >> r) & 1;
                    /* forbid horizontal 000 and 010 */
                    if ((a==0 && b==0 && c==0) || (a==0 && b==1 && c==0))
                        ok = false;
                }
                if (ok) {
                    int v = j * col_cnt + k;
                    next_idx[idx][ next_len[idx]++ ] = v;
                }
            }
        }
    }

    trans_init = true;
}

/* -------------------------------------------------------------------------- */
/*  Sequence function a(n)                                                   */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return (seq_t)0;   /* offset is 1, value unused */
    if (n == 1) return (seq_t)col_cnt;
    if (n == 2) return (seq_t)col_cnt * (seq_t)col_cnt;

    /* DP arrays for states (prev_col, cur_col) */
    static seq_t dp[MAX_STATES];
    static seq_t ndp[MAX_STATES];

    int state_cnt = col_cnt * col_cnt;

    /* initialise for width 2: all pairs are allowed */
    for (int s = 0; s < state_cnt; ++s) dp[s] = 1;

    /* extend to width n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int s = 0; s < state_cnt; ++s) ndp[s] = 0;
        for (int u = 0; u < state_cnt; ++u) {
            seq_t val = dp[u];
            if (val == 0) continue;
            unsigned lc = next_len[u];
            for (unsigned t = 0; t < lc; ++t) {
                int v = next_idx[u][t];
                ndp[v] += val;
            }
        }
        for (int s = 0; s < state_cnt; ++s) dp[s] = ndp[s];
    }

    /* sum over all terminal states */
    seq_t total = 0;
    for (int s = 0; s < state_cnt; ++s) total += dp[s];
    return total;
}
