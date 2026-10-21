/*
 * A207370  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 21, u64 n <= 55, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of 5×n binary arrays avoiding horizontal 000 and 011
   and vertical 001 and 011.
   Computed by dynamic programming over columns with a state consisting of
   the last two columns.  No table of values is hard‑coded; the recurrence
   is built from the constraints themselves. */
/* External types provided by the build system */
#include <stdbool.h>
#include <stdint.h>          /* for uint8_t */

/* ------------------------------------------------------------------ */
/* Precomputation tables ------------------------------------------------ */

static bool vertical_valid[32];          /* does a column satisfy vertical rules? */
static uint8_t trans_list[1024][32];     /* allowed next columns for each pair state */
static uint8_t trans_count[1024];        /* number of valid transitions from each state */
static bool init_done = false;

/* ------------------------------------------------------------------ */
static void init_tables(void)
{
    int i, j, k, r;

    /* 1. vertical rule: no 001 or 011 in any 3 consecutive rows of a column */
    for (i = 0; i < 32; ++i) {
        bool ok = true;
        for (r = 0; r < 3 && ok; ++r) {
            int bits = (i >> r) & 7;     /* 3‑bit pattern rows r,r+1,r+2 */
            if (bits == 0b001 || bits == 0b011)
                ok = false;
        }
        vertical_valid[i] = ok;
    }

    /* 2. transition table between pairs of columns according to horizontal rules */
    for (i = 0; i < 32; ++i) {
        for (j = 0; j < 32; ++j) {
            int idx = (i << 5) | j;     /* state (prev2,prev1) */
            if (!vertical_valid[i] || !vertical_valid[j]) {
                trans_count[idx] = 0;
                continue;
            }
            uint8_t cnt = 0;
            for (k = 0; k < 32; ++k) {
                if (!vertical_valid[k]) continue;
                bool h_ok = true;
                for (r = 0; r < 5 && h_ok; ++r) {
                    int b2 = (i >> r) & 1;
                    int b1 = (j >> r) & 1;
                    int b0 = (k >> r) & 1;
                    if (b2 == 0 && b1 == 0 && b0 == 0)
                        h_ok = false;
                    else if (b2 == 0 && b1 == 1 && b0 == 1)
                        h_ok = false;
                }
                if (h_ok) {
                    trans_list[idx][cnt++] = (uint8_t)k;
                }
            }
            trans_count[idx] = cnt;
        }
    }
    init_done = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done) init_tables();           /* ensure tables exist */

    /* count of valid columns (height 5) */
    unsigned V_int = 0;
    for (int c = 0; c < 32; ++c)
        if (vertical_valid[c]) ++V_int;
    seq_t V = (seq_t)V_int;

    if (n == 0) return (seq_t)0;             /* not part of OEIS, safe guard */

    if (n == 1) return V;
    if (n == 2) return V * V;                /* all pairs allowed for n<3 */

    /* DP arrays: 32×32 states */
    static seq_t dp[1024];
    static seq_t newdp[1024];

    /* initialise dp for the first two columns */
    for (int s = 0; s < 1024; ++s) dp[s] = (seq_t)0;
    for (int c1 = 0; c1 < 32; ++c1) if (vertical_valid[c1]) {
        for (int c2 = 0; c2 < 32; ++c2) if (vertical_valid[c2]) {
            dp[(c1 << 5) | c2] = (seq_t)1;
        }
    }

    /* iterate through columns 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int s = 0; s < 1024; ++s) newdp[s] = (seq_t)0;

        for (int s = 0; s < 1024; ++s) {
            seq_t cur = dp[s];
            if (cur == (seq_t)0) continue;
            int prev1 = s & 31;
            uint8_t *lst = trans_list[s];
            uint8_t cnt = trans_count[s];
            for (int t = 0; t < cnt; ++t) {
                uint8_t nxt = lst[t];
                int dest = (prev1 << 5) | nxt;
                newdp[dest] += cur;
            }
        }
        /* copy newdp back to dp for next step */
        for (int s = 0; s < 1024; ++s) dp[s] = newdp[s];
    }

    /* sum over all final states to get the answer */
    seq_t ans = (seq_t)0;
    for (int s = 0; s < 1024; ++s)
        ans += dp[s];

    return ans;
}
