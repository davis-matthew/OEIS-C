/*
 * A207742  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming over column pairs a(n) = number
 *            of 4×n 0/1 arrays avoiding horizontally 0 0 0 and 0 1 0,
 *            vertically 0 0 1 and 1 0 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* constant limits – compile‑time constants, no VLA issues */
enum { MAXC = 16,               /* 2⁴ possible columns          */
       MAXS = MAXC * MAXC,      /* all column‑pair states       */
       MAXTRANS = MAXC };       /* maximal outgoing edges      */

static bool        initialized = false;
static seq_t       a1, a2;      /* base values a(1), a(2) */

/* precomputed tables – all sizes known at compile time */
static bool   vertical_ok[MAXC];
static int    allowed_cols[MAXC], n_allowed;
static int    state_idx[MAXC][MAXC];
static int    state_col[MAXS][2];
static int    state_count;

static int    adj[MAXS][MAXTRANS];
static int    adj_sz[MAXS];

static seq_t  dp_cur[MAXS];
static seq_t  dp_next[MAXS];

/* build all tables once, on first call */
static void init(void)
{
    int i, j, t, s;

    /* columns that satisfy the vertical rule (no 001 or 100 in overlapping triples) */
    n_allowed = 0;
    for (i = 0; i < MAXC; ++i) {
        bool ok = true;
        for (j = 0; j <= 1; ++j) {          /* check triples (0,1,2) and (1,2,3) */
            int trip = (i >> j) & 7;        /* three consecutive bits */
            if (trip == 0b001 || trip == 0b100) {
                ok = false;
                break;
            }
        }
        vertical_ok[i] = ok;
        if (ok) allowed_cols[n_allowed++] = i;
    }

    /* map each allowed pair to a state index */
    state_count = 0;
    for (i = 0; i < MAXC; ++i)
        for (j = 0; j < MAXC; ++j)
            state_idx[i][j] = -1;
    for (i = 0; i < n_allowed; ++i)
        for (j = 0; j < n_allowed; ++j) {
            state_col[state_count][0] = allowed_cols[i];
            state_col[state_count][1] = allowed_cols[j];
            state_idx[allowed_cols[i]][allowed_cols[j]] = state_count;
            ++state_count;
        }

    /* precompute transitions: from (c1,c2) → (c2,c3) that respect horizontal rules */
    for (s = 0; s < state_count; ++s) {
        int c1 = state_col[s][0];
        int c2 = state_col[s][1];
        adj_sz[s] = 0;
        for (t = 0; t < n_allowed; ++t) {
            int c3 = allowed_cols[t];
            bool ok = true;
            for (i = 0; i < 4; ++i) {
                int a = (c1 >> i) & 1;
                int b = (c2 >> i) & 1;
                int c = (c3 >> i) & 1;
                int trip = (a << 2) | (b << 1) | c;
                if (trip == 0b000 || trip == 0b010) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            int dest = state_idx[c2][c3];
            if (dest >= 0) {
                adj[s][adj_sz[s]++] = dest;
            }
        }
    }

    /* base values */
    a1 = (seq_t)n_allowed;    /* single column: any allowed column works */
    a2 = a1 * a1;             /* two columns: any pair works (no horizontal check needed) */

    initialized = true;
}

/* public function required by the harness */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 1) return a1;
    if (n == 2) return a2;

    /* start DP for arrays of length 2: every state occurs once */
    for (int s = 0; s < state_count; ++s) dp_cur[s] = (seq_t)1;

    /* extend arrays step by step */
    for (idx_t step = 3; step <= n; ++step) {
        for (int s = 0; s < state_count; ++s) dp_next[s] = (seq_t)0;
        for (int s = 0; s < state_count; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            for (int k = 0; k < adj_sz[s]; ++k) {
                int dest = adj[s][k];
                dp_next[dest] += val;
            }
        }
        for (int s = 0; s < state_count; ++s) dp_cur[s] = dp_next[s];
    }

    /* sum all states to get the total number of arrays of width n */
    seq_t res = (seq_t)0;
    for (int s = 0; s < state_count; ++s) res += dp_cur[s];
    return res;
}
