/*
 * A207521  Number of 5 X n 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A207521: the number of 5×n binary arrays avoiding
 * the patterns 0 0 1 and 0 1 0 horizontally and 0 1 0 and 1 0 1 vertically.
 * The algorithm uses a transfer‑matrix dynamic programming over column pairs
 * with precompiled validity tables.  It works for all supported seq_t /
 * idx_t types and is O(n·states) with < 2 000 states. */

#include <stdint.h>
#include <stddef.h>

/* ----------  precomputation tables  ---------- */
#define MAX_COLS   32
#define MAX_STATES (MAX_COLS * MAX_COLS)          /* 1024 */

static int Vlist[MAX_COLS];          /* list of vertical‑valid columns     */
static int Vsize;                    /* number of vertical‑valid columns   */
static int pair_idx[MAX_COLS][MAX_COLS]; /* index of state (p,q)              */
static int state_count;                 /* number of states = Vsize²           */
static int next_state[MAX_STATES][MAX_COLS]; /* adjacency list                  */
static int next_count[MAX_STATES];          /* number of successors per state    */
static int init_done;                     /* flag for one‑time initialisation   */

/* ----------  helper for initialization  ---------- */
static void build(void)
{
    if (init_done) return;
    init_done = 1;

    /* ----  vertical‑valid columns (no 010 or 101 within one column) ---- */
    Vsize = 0;
    for (int col = 0; col < 32; ++col) {
        int ok = 1;
        for (int i = 0; i < 3; ++i) { /* triples of rows i,i+1,i+2   */
            int a = (col >> i)       & 1;
            int b = (col >> (i + 1)) & 1;
            int c = (col >> (i + 2)) & 1;
            if ((a == 0 && b == 1 && c == 0) || /* 010   */
                (a == 1 && b == 0 && c == 1))  /* 101   */
            { ok = 0; break; }
        }
        if (ok) Vlist[Vsize++] = col;
    }

    /* ----  state indices for all ordered pairs of valid columns ---- */
    state_count = 0;
    for (int i = 0; i < MAX_COLS; ++i)
        for (int j = 0; j < MAX_COLS; ++j)
            pair_idx[i][j] = -1;
    for (int i = 0; i < Vsize; ++i)
        for (int j = 0; j < Vsize; ++j)
            pair_idx[Vlist[i]][Vlist[j]] = state_count++;

    /* ----  transition tables: (p,q) -> (q,r) ---- */
    for (int s = 0; s < state_count; ++s) next_count[s] = 0;
    for (int i = 0; i < Vsize; ++i) {
        int p = Vlist[i];
        for (int j = 0; j < Vsize; ++j) {
            int q = Vlist[j];
            int s = pair_idx[p][q];
            for (int k = 0; k < Vsize; ++k) {
                int r = Vlist[k];
                /* check horizontal constraints on all rows */
                int ok = 1;
                for (int bit = 0; bit < 5; ++bit) {
                    int a = (p >> bit) & 1;
                    int b = (q >> bit) & 1;
                    int c = (r >> bit) & 1;
                    if ((a == 0 && b == 0 && c == 1) || /* 001   */
                        (a == 0 && b == 1 && c == 0))   /* 010   */
                    { ok = 0; break; }
                }
                if (ok) {
                    int t = pair_idx[q][r];
                    next_state[s][next_count[s]++] = t;
                }
            }
        }
    }
}

/* ----------  main function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    build();

    /* --- handle small n explicitly --- */
    if (n == 1) return (seq_t)Vsize;          /* single column                */
    if (n == 2) {                            /* any pair of vertical columns */
        seq_t res = 1;
        res *= (seq_t)Vsize;
        res *= (seq_t)Vsize;
        return res;
    }

    /* --- DP over column pairs for n >= 3 --- */
    static seq_t dp[MAX_STATES];
    static seq_t newdp[MAX_STATES];
    /* initialise dp with all states = 1 (sequence of length 2) */
    for (int i = 0; i < state_count; ++i) dp[i] = (seq_t)1;

    for (idx_t step = 3; step <= n; ++step) {
        /* clear newdp */
        for (int i = 0; i < state_count; ++i) newdp[i] = (seq_t)0;

        /* transition */
        for (int s = 0; s < state_count; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;
            int cnt = next_count[s];
            for (int idx = 0; idx < cnt; ++idx) {
                int t = next_state[s][idx];
                newdp[t] += val;
            }
        }
        /* copy newdp back to dp */
        for (int i = 0; i < state_count; ++i) dp[i] = newdp[i];
    }

    /* sum all states for the total number of arrays of width n */
    seq_t total = (seq_t)0;
    for (int i = 0; i < state_count; ++i) total += dp[i];
    return total;
}
