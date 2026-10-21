/*
 * A234906  Number of (n+1) X (2+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays of 0..7 with no equal
 *            horizontally or vertically adjacent elements and each 2×2
 *            sub‑block having the sums of its diagonals differing by exactly
 *            1. Computed by a dynamic‑programming walk over 392 possible rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define NUM_STATES 392U
#define MAX_TRANS  NUM_STATES

/* State representation: each row is a triple of 0..7 with no equal adjacent elements. */
static unsigned char state_vals[NUM_STATES][3];
/* Transition adjacency list: for each state s, trans_to[s][k] are targets t. */
static unsigned short trans_to[NUM_STATES][MAX_TRANS];
static unsigned char trans_len[NUM_STATES];

/* One‑time initialization of states and adjacency list. */
static void init_adj(void)
{
    static int done = 0;
    if (done) return;
    /* generate all valid rows */
    int s = 0;
    for (int v0 = 0; v0 < 8; ++v0)
        for (int v1 = 0; v1 < 8; ++v1)
            if (v1 != v0)
                for (int v2 = 0; v2 < 8; ++v2)
                    if (v2 != v1)
                    {
                        state_vals[s][0] = (unsigned char)v0;
                        state_vals[s][1] = (unsigned char)v1;
                        state_vals[s][2] = (unsigned char)v2;
                        ++s;
                    }
    /* build adjacency lists */
    for (int ps = 0; ps < NUM_STATES; ++ps)
    {
        trans_len[ps] = 0;
        int p0 = state_vals[ps][0];
        int p1 = state_vals[ps][1];
        int p2 = state_vals[ps][2];
        for (int cs = 0; cs < NUM_STATES; ++cs)
        {
            int c0 = state_vals[cs][0];
            int c1 = state_vals[cs][1];
            int c2 = state_vals[cs][2];
            /* vertical adjacency */
            if (c0 == p0 || c1 == p1 || c2 == p2) continue;
            /* 2×2 block diagonal sum condition */
            int d1 = (p0 + c1) - (p1 + c0);
            if (d1 < 0) d1 = -d1;
            if (d1 != 1) continue;
            int d2 = (p1 + c2) - (p2 + c1);
            if (d2 < 0) d2 = -d2;
            if (d2 != 1) continue;
            trans_to[ps][trans_len[ps]++] = (unsigned short)cs;
        }
    }
    done = 1;
}

seq_t a(idx_t n)
{
    init_adj();

    if (n == 0) return 0;          /* not used for the official offset (1) */

    static seq_t cur[NUM_STATES];
    static seq_t nxt[NUM_STATES];
    /* start with any first row */
    for (int i = 0; i < NUM_STATES; ++i) cur[i] = 1;

    for (idx_t step = 0; step < n; ++step)
    {
        /* reset next state */
        for (int i = 0; i < NUM_STATES; ++i) nxt[i] = 0;
        /* propagate counts */
        for (int s = 0; s < NUM_STATES; ++s)
        {
            if (cur[s] == 0) continue;
            seq_t add = cur[s];
            unsigned char len = trans_len[s];
            unsigned short const *targets = trans_to[s];
            for (int k = 0; k < len; ++k)
                nxt[targets[k]] += add;
        }
        /* copy next to current for next iteration */
        for (int i = 0; i < NUM_STATES; ++i) cur[i] = nxt[i];
    }
    /* total number of arrays */
    seq_t res = 0;
    for (int i = 0; i < NUM_STATES; ++i) res += cur[i];
    return res;
}
