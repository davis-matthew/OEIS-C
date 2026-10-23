/*
 * A145415  Number of 2-factors in P_7 X P_2n.
 * https://oeis.org/A145415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of 2‑factors in the grid graph P_7 × P_{2n}
 *            (computed by a transfer‑matrix DP over the 7 rows)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for static bool */

/* Transition tables between DP states. */
static unsigned char trans[128][128];          /* raw counts for each s→t */
static unsigned char trans_t[128][64];         /* destination indices for each s */
static unsigned char trans_count[128][64];     /* multiplicity of each transition */
static unsigned char trans_len[128];           /* number of non‑zero transitions from state s */

/* Build the transfer matrix once.  The matrix size is 128×128 because
   a state is a 7‑bit pattern of pending horizontal edges.  The number
   of outgoing transitions from any state is at most 64 (one for each
   vertical edge pattern).  */
static void init_trans(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Compute all transitions by brute force over vertical masks. */
    for (int s = 0; s < 128; ++s) {
        int rem[7];
        for (int r = 0; r < 7; ++r)
            rem[r] = 2 - ((s >> r) & 1);   /* remaining degree needed in this column */

        /* Enumerate all 6‑bit masks representing vertical edges in the column. */
        for (int mask = 0; mask < (1 << 6); ++mask) {
            int vert[7] = {0};
            for (int i = 0; i < 7; ++i) {
                int v = 0;
                if (i > 0)   v += (mask >> (i - 1)) & 1;
                if (i < 6)   v += (mask >> i) & 1;
                vert[i] = v;
            }

            bool ok = true;
            int next_s = 0;   /* outgoing horizontal edges to the right */
            for (int i = 0; i < 7; ++i) {
                int need = rem[i];
                /* need = vert[i] + horiz[i], where horiz[i] is 0 or 1. */
                if (vert[i] < need - 1 || vert[i] > need) {
                    ok = false;
                    break;
                }
                int horiz = need - vert[i];
                if (horiz) next_s |= 1 << i;
            }

            if (ok) {
                unsigned char *p = &trans[s][next_s];
                *p = (unsigned char)(*p + 1);
            }
        }

        /* Record non‑zero transitions for fast DP updates. */
        unsigned char *tptr   = trans_t[s];
        unsigned char *cptr   = trans_count[s];
        unsigned char count = 0;
        for (int t = 0; t < 128; ++t) {
            unsigned char v = trans[s][t];
            if (v) {
                tptr[count]   = (unsigned char)t;
                cptr[count++] = v;
            }
        }
        trans_len[s] = count;
    }
}

/* Big‑int DP over columns. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* undefined, but harmless */

    idx_t width = 2 * n;      /* number of columns in the grid */

    init_trans();             /* ensure transition tables are ready */

    seq_t dp[128];
    seq_t ndp[128];

    /* initialise dp array to zero */
    for (int i = 0; i < 128; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;             /* start with no pending edges */

    for (idx_t col = 0; col < width; ++col) {
        for (int i = 0; i < 128; ++i) ndp[i] = (seq_t)0;

        for (int s = 0; s < 128; ++s) {
            seq_t val = dp[s];
            if (val == (seq_t)0) continue;

            unsigned char len = trans_len[s];
            unsigned char *tp = trans_t[s];
            unsigned char *cp = trans_count[s];

            for (unsigned char i = 0; i < len; ++i) {
                int t    = tp[i];
                int mult = cp[i];
                ndp[t] = ndp[t] + val * (seq_t)mult;
            }
        }
        /* move to the next column */
        for (int i = 0; i < 128; ++i) dp[i] = ndp[i];
    }

    return dp[0];    /* no horizontal edges pending after the last column */
}
