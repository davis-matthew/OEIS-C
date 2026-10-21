/*
 * A224052  Number of nX3 0..2 arrays with rows and columns unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A224052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 22, u64 n <= 160, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A224052: number of n×3 arrays with entries 0..2,
   rows and columns unimodal, and antidiagonals nondecreasing.
   Uses dynamic programming over row values and per‑column descending flags. */
#include <stdbool.h>

#define STATE_COUNT 216             /* 3^3 * 2^3 possible combinations of (values, flags) */
static int  state_v[STATE_COUNT][3];
static int  state_d[STATE_COUNT][3];
static bool state_allowed[STATE_COUNT];
static int  allowed_indices[STATE_COUNT];
static int  allowed_count = 0;
static int  transition[STATE_COUNT][STATE_COUNT];
static int  trans_count[STATE_COUNT] = {0};
static int  triple_to_state[3][3][3];   /* index for flags=0/0/0 */

/* initialise lookup tables (only once) */
static void init_tables(void)
{
    if (allowed_count != 0) return;   /* already initialised */

    int idx = 0;
    for (int v1 = 0; v1 < 3; ++v1)
        for (int v2 = 0; v2 < 3; ++v2)
            for (int v3 = 0; v3 < 3; ++v3)
                for (int d1 = 0; d1 < 2; ++d1)
                    for (int d2 = 0; d2 < 2; ++d2)
                        for (int d3 = 0; d3 < 2; ++d3)
                        {
                            state_v[idx][0]   = v1;
                            state_v[idx][1]   = v2;
                            state_v[idx][2]   = v3;
                            state_d[idx][0]   = d1;
                            state_d[idx][1]   = d2;
                            state_d[idx][2]   = d3;
                            /* unimodal row: forbid valley a>b<c */
                            state_allowed[idx] = !(v1 > v2 && v2 < v3);
                            if (state_allowed[idx])
                                allowed_indices[allowed_count++] = idx;
                            if (d1 == 0 && d2 == 0 && d3 == 0)
                                triple_to_state[v1][v2][v3] = idx;
                            ++idx;
                        }

    /* build compatible transitions */
    for (int i = 0; i < STATE_COUNT; ++i)
    {
        if (!state_allowed[i]) continue;
        for (int j = 0; j < STATE_COUNT; ++j)
        {
            if (!state_allowed[j]) continue;

            int v1  = state_v[i][0], v2  = state_v[i][1], v3  = state_v[i][2];
            int nv1 = state_v[j][0], nv2 = state_v[j][1], nv3 = state_v[j][2];
            int d1 = state_d[i][0], d2 = state_d[i][1], d3 = state_d[i][2];

            /* antidiagonal constraints: (i-1,2) <= (i,1) and (i-1,3) <= (i,2) */
            if (v2 > nv1) continue;
            if (v3 > nv2) continue;

            int nd1, nd2, nd3;
            /* column 1 descent logic */
            if (d1 == 0)
                nd1 = (nv1 < v1) ? 1 : 0;
            else { if (nv1 > v1) continue; nd1 = 1; }

            /* column 2 descent logic */
            if (d2 == 0)
                nd2 = (nv2 < v2) ? 1 : 0;
            else { if (nv2 > v2) continue; nd2 = 1; }

            /* column 3 descent logic */
            if (d3 == 0)
                nd3 = (nv3 < v3) ? 1 : 0;
            else { if (nv3 > v3) continue; nd3 = 1; }

            /* destination state must carry the same remaining flags */
            if (state_d[j][0] != nd1) continue;
            if (state_d[j][1] != nd2) continue;
            if (state_d[j][2] != nd3) continue;

            transition[i][trans_count[i]++] = j;
        }
    }
}

seq_t a(idx_t n)
{
    static bool init_done = false;
    if (!init_done) { init_tables(); init_done = true; }

    static seq_t dp_prev[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];

    /* clear previous DP buffer */
    for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = 0;

    /* initialise first row: only the flag‑free states are possible initially */
    for (int v1 = 0; v1 < 3; ++v1)
        for (int v2 = 0; v2 < 3; ++v2)
            for (int v3 = 0; v3 < 3; ++v3)
            {
                if (!(v1 > v2 && v2 < v3)) /* row unimodal */
                {
                    int idx = triple_to_state[v1][v2][v3];
                    dp_prev[idx] = 1;      /* each allowed row contributes one array */
                }
            }

    /* process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row)
    {
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        for (int si = 0; si < allowed_count; ++si)
        {
            int i          = allowed_indices[si];
            seq_t cur_entry = dp_prev[i];
            if (cur_entry == 0) continue;
            int tcnt = trans_count[i];
            for (int t = 0; t < tcnt; ++t)
            {
                int j = transition[i][t];
                dp_next[j] += cur_entry;
            }
        }
        /* move to next row */
        for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = dp_next[i];
    }

    /* sum over all acceptable final states */
    seq_t result = 0;
    for (int s = 0; s < allowed_count; ++s)
        result += dp_prev[allowed_indices[s]];

    return result;
}
