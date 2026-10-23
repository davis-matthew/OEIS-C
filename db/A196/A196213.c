/*
 * A196213  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 3,0,1,2,4 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays with entries 0..4 such that each
 *            element x equals the number of its horizontal and vertical
 *            neighbours that are equal to t(x), where t = {3,0,1,2,4} for x =
 *            0,1,2,3,4. The computation is performed by dynamic programming
 *            over pairs of consecutive rows
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   seq_alloc_array, seq_set_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* The utilities provided by the setting allow allocation of arrays
   of the arbitrary‐precision type seq_t.  */
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_set_zero.h"

#define ROWS 125                 /* 5^3 possible rows */
#define STATE (ROWS * ROWS)

static int          row_vals[ROWS][3];
static const int    ct[5] = {3,0,1,2,4};

static bool         valid_first[ROWS][ROWS];   /* first row given bottom */
static bool         valid_last [ROWS][ROWS];   /* last row given top    */
static unsigned short next_cnt[STATE];
static unsigned short next_rows[STATE][ROWS];   /* next rows for a state */

/* ------------------------------------------------------------------ */
/*  Helper to test whether a row satisfies its local constraint       */
static bool
check_row (int cur, int top, int bot)
{
    int c, x, t, cnt;
    for (c = 0; c < 3; ++c)
    {
        x = row_vals[cur][c];
        t = ct[x];
        cnt = 0;

        /* left neighbour */
        if (c > 0 && row_vals[cur][c-1] == t) ++cnt;
        /* right neighbour */
        if (c < 2 && row_vals[cur][c+1] == t) ++cnt;
        /* top neighbour */
        if (top  >= 0 && row_vals[top][c] == t) ++cnt;
        /* bottom neighbour */
        if (bot  >= 0 && row_vals[bot][c] == t) ++cnt;

        if (cnt != x) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Pre‑computation of all tables once                          */
static bool precomp_done = false;

static void
init_precomp (void)
{
    int i, j, k, idx;
    /* all row vectors */
    for (i = 0; i < 5; ++i)
        for (j = 0; j < 5; ++j)
            for (k = 0; k < 5; ++k)
            {
                idx = i*25 + j*5 + k;           /* 0 … 124 */
                row_vals[idx][0] = i;
                row_vals[idx][1] = j;
                row_vals[idx][2] = k;
            }

    /* first and last row checks */
    for (i = 0; i < ROWS; ++i)
        for (j = 0; j < ROWS; ++j)
        {
            valid_first[i][j] = check_row (i, -1, j);      /* top missing */
            valid_last [i][j] = check_row (j, i, -1);      /* bottom missing */
        }

    /* transition lists: from (prev,curr) -> next rows that satisfy curr */
    for (i = 0; i < ROWS; ++i)
        for (j = 0; j < ROWS; ++j)
        {
            unsigned short cnt = 0;
            int s = i*ROWS + j;
            for (k = 0; k < ROWS; ++k)
                if (check_row (j, i, k))
                    next_rows[s][cnt++] = k;
            next_cnt[s] = cnt;
        }

    precomp_done = true;
}

/* ------------------------------------------------------------------ */
/*  Allocate and initialise two DP arrays, using the provided functor   */
static seq_t *dp_prev = NULL;       /* values for (prev,curr) pairs   */
static seq_t *dp_next = NULL;       /* scratch buffer                 */
static bool   dp_alloced = false;

static void
alloc_dp (void)
{
    if (!dp_alloced)
    {
        dp_prev  = seq_alloc_array (STATE);
        dp_next  = seq_alloc_array (STATE);
        dp_alloced = true;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                        */
seq_t
a (idx_t n)
{
    if (!precomp_done) init_precomp ();
    if (!dp_alloced)   alloc_dp       ();

    /* n = 0 is irrelevant.  */
    if (n == 0) return 0;

    /* n = 1: single row with no vertical neighbours */
    if (n == 1)
    {
        seq_t cnt = 0;
        int i;
        for (i = 0; i < ROWS; ++i)
            if (check_row (i, -1, -1))
                cnt = cnt + 1;
        return cnt;
    }

    /* initialise DP with pairs (row1,row2) that satisfy row1 constraint */
    seq_set_zero (dp_prev, STATE);
    seq_set_zero (dp_next, STATE);

    {
        int i, j, idx;
        for (i = 0; i < ROWS; ++i)
            for (j = 0; j < ROWS; ++j)
                if (valid_first[i][j])
                {
                    idx = i*ROWS + j;
                    dp_prev[idx] = dp_prev[idx] + 1;
                }
    }

    if (n == 2)
    {
        seq_t result = 0;
        int i, j, idx;
        for (i = 0; i < ROWS; ++i)
            for (j = 0; j < ROWS; ++j)
            {
                idx = i*ROWS + j;
                seq_t val = dp_prev[idx];
                if (val != 0 && valid_last[i][j])
                    result = result + val;
            }
        return result;
    }

    /* n >= 3: iterate n-2 transitions */
    {
        unsigned int step;
        for (step = 3; step <= (unsigned int)n; ++step)
        {
            /* compute dp_next from dp_prev */
            seq_set_zero (dp_next, STATE);
            unsigned int s;
            for (s = 0; s < STATE; ++s)
            {
                seq_t val = dp_prev[s];
                if (val == 0) continue;

                int prev = s / ROWS;
                int curr = s % ROWS;
                unsigned short cnt = next_cnt[s];
                unsigned short *list = next_rows[s];
                unsigned short t;
                for (t = 0; t < cnt; ++t)
                {
                    int nxt = list[t];
                    int idx_next = curr*ROWS + nxt;
                    dp_next[idx_next] = dp_next[idx_next] + val;
                }
            }
            /* swap buffers */
            {
                seq_t *tmp = dp_prev;
                dp_prev     = dp_next;
                dp_next     = tmp;
            }
        }
    }

    /* final summation: enforce last row constraint */
    seq_t result = 0;
    {
        unsigned int s;
        for (s = 0; s < STATE; ++s)
        {
            seq_t val = dp_prev[s];
            if (val == 0) continue;
            int prev = s / ROWS;
            int curr = s % ROWS;
            if (valid_last[prev][curr])
                result = result + val;
        }
    }
    return result;
}
