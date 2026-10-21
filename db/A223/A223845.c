/*
 * A223845  Number of nX3 0..3 arrays with rows and columns unimodal.
 * https://oeis.org/A223845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×3 arrays over the alphabet {0,1,2,3} such * that
 *            every row and every column is unimodal. * Computation is
 *            performed by dynamic programming over 512 states. * A state
 *            encodes the values of the last row (each 0–3) and for * each
 *            column a flag that tells whether the column has already *...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 35, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Constants and tables                                               */
/* ------------------------------------------------------------------ */
#define STATES   512
#define N_ROWS   50   /* number of unimodal triples */

/* All unimodal triples (a,b,c) with values in [0,3] */
static unsigned char unimodal[N_ROWS][3];
static int unimodal_cnt = 0;

/* Transition adjacency lists.  For each state there are at most
 * N_ROWS outgoing edges.  The list may contain duplicate destinations
 * because different row triples can lead to the same next state.
 */
static unsigned short adj[STATES][N_ROWS];
static unsigned char  adj_deg[STATES];

/* Flag showing whether the data tables have been initialized. */
static int data_initialized = 0;
static int adj_initialized = 0;

/* ------------------------------------------------------------------ */
/* Helper functions                                                   */
/* ------------------------------------------------------------------ */

/* Encode a state from the three values (0–3) and three flags (0/1). */
static inline int encode_state(unsigned char v0, unsigned char v1,
                               unsigned char v2,
                               unsigned char f0, unsigned char f1, unsigned char f2)
{
    return (v0 | (v1 << 2) | (v2 << 4)) |
           ((f0 & 1) << 6) |
           ((f1 & 1) << 7) |
           ((f2 & 1) << 8);
}

/* Decode a state into the three values and flags. */
static inline void decode_state(int state,
                                unsigned char *v0, unsigned char *v1, unsigned char *v2,
                                unsigned char *f0, unsigned char *f1, unsigned char *f2)
{
    *v0 = (state & 3);
    *v1 = (state >> 2) & 3;
    *v2 = (state >> 4) & 3;
    *f0 = (state >> 6) & 1;
    *f1 = (state >> 7) & 1;
    *f2 = (state >> 8) & 1;
}

/* ------------------------------------------------------------------ */
/* Data initialization                                               */
/* ------------------------------------------------------------------ */

/* Build the list of all unimodal triples. */
static void init_unimodal(void)
{
    if (unimodal_cnt > 0) return;
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c)
            {
                if (b < a && b < c)    /* valley: not unimodal */
                    continue;
                unimodal[unimodal_cnt][0] = (unsigned char)a;
                unimodal[unimodal_cnt][1] = (unsigned char)b;
                unimodal[unimodal_cnt][2] = (unsigned char)c;
                ++unimodal_cnt;
            }
}

/* Build adjacency lists for the transition graph.  Each edge
 * represents a valid transition between two successive rows.
 */
static void init_adjacency(void)
{
    if (adj_initialized) return;
    init_unimodal();
    for (int s = 0; s < STATES; ++s)
    {
        unsigned char v0, v1, v2, f0, f1, f2;
        decode_state(s, &v0, &v1, &v2, &f0, &f1, &f2);
        unsigned char nf0, nf1, nf2;
        unsigned char nv0, nv1, nv2;
        int deg = 0;
        for (int t = 0; t < unimodal_cnt; ++t)
        {
            nv0 = unimodal[t][0];
            nv1 = unimodal[t][1];
            nv2 = unimodal[t][2];
            /* Determine new flags and check feasibility. */
            nf0 = nf1 = nf2 = 0;
            int valid = 1;
            /* Column 0 */
            if (f0 == 0)
            {
                if (nv0 < v0) nf0 = 1;
                else nf0 = 0;
            }
            else
            {
                if (nv0 > v0) valid = 0;
                else nf0 = 1;
            }
            /* Column 1 */
            if (valid)
            {
                if (f1 == 0)
                {
                    if (nv1 < v1) nf1 = 1;
                    else nf1 = 0;
                }
                else
                {
                    if (nv1 > v1) valid = 0;
                    else nf1 = 1;
                }
            }
            /* Column 2 */
            if (valid)
            {
                if (f2 == 0)
                {
                    if (nv2 < v2) nf2 = 1;
                    else nf2 = 0;
                }
                else
                {
                    if (nv2 > v2) valid = 0;
                    else nf2 = 1;
                }
            }
            if (!valid) continue;
            int dest = encode_state(nv0, nv1, nv2, nf0, nf1, nf2);
            adj[s][deg++] = (unsigned short)dest;
        }
        adj_deg[s] = deg;
    }
    adj_initialized = 1;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n==0, return 0 for safety */
    /* Ensure tables are ready. */
    init_adjacency();

    /* DP arrays */
    static seq_t dp[STATES];
    static seq_t next[STATES];
    /* Zero dp */
    for (int i = 0; i < STATES; ++i) dp[i] = 0;

    /* First row: any unimodal triple, no columns have decreased yet. */
    for (int t = 0; t < unimodal_cnt; ++t)
    {
        int state = encode_state(unimodal[t][0], unimodal[t][1], unimodal[t][2],
                                 0, 0, 0);
        dp[state] += 1;   /* dp[state] += 1 */
    }

    /* If only one row, return total count (which equals 50). */
    if (n == 1)
    {
        seq_t result = 0;
        for (int i = 0; i < STATES; ++i)
            result += dp[i];
        return result;
    }

    /* Process subsequent rows. */
    for (idx_t row = 2; row <= n; ++row)
    {
        /* Zero next */
        for (int i = 0; i < STATES; ++i) next[i] = 0;

        /* Transition */
        for (int s = 0; s < STATES; ++s)
        {
            seq_t cur = dp[s];
            if (cur == 0) continue;
            unsigned char deg = adj_deg[s];
            unsigned short *dsts = adj[s];
            for (unsigned char k = 0; k < deg; ++k)
                next[dsts[k]] += cur;
        }

        /* Prepare for next iteration: dp = next */
        for (int i = 0; i < STATES; ++i)
            dp[i] = next[i];
    }

    /* Sum all final states to obtain the result. */
    seq_t result = 0;
    for (int i = 0; i < STATES; ++i)
        result += dp[i];
    return result;
}
