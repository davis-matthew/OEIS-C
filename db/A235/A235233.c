/*
 * A235233  Number of (n+1) X (2+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 6,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235233: Number of (n+1)×3 0..6 arrays with the
 * diagonal‑antidiagonal constraint and no equal adjacent elements.
 *
 * The number is computed by dynamic programming over all admissible
 * 3‑digit rows.  For every row we pre‑compute the list of rows that
 * can follow it; the recurrence counts how many ways to reach each row
 * after i rows.  Finally the sum of all counts after n transitions
 * (i.e. after n+1 rows) is returned.
 *
 * Only integer arithmetic is used; the code is valid in C11 and C++17.
 * No <math.h> or floating point operations appear.
 *
 * Since seq_t may be an arbitrary‑precision class its objects are
 * manipulated only by assignment; memcpy is not used on seq_t arrays.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define STATE_COUNT 252  /* all 3‑digit rows with no horizontal equal */

static int row_vals[STATE_COUNT][3];          /* digit triples */
static short trans_next[STATE_COUNT][STATE_COUNT];
static unsigned short trans_cnt[STATE_COUNT];
static bool tables_inited = false;

/* Construct all admissible rows (no two neighboring digits equal) */
static void build_rows(void)
{
    int idx = 0;
    for (int a = 0; a <= 6; ++a)
        for (int b = 0; b <= 6; ++b)
            if (b != a)
                for (int c = 0; c <= 6; ++c)
                    if (c != b)
                    {
                        row_vals[idx][0] = a;
                        row_vals[idx][1] = b;
                        row_vals[idx][2] = c;
                        ++idx;
                    }
}

/* Test that row q may follow row p */
static bool valid_transition(int p, int q)
{
    /* No vertical equal adjacent cells */
    if (row_vals[p][0] == row_vals[q][0]) return false;
    if (row_vals[p][1] == row_vals[q][1]) return false;
    if (row_vals[p][2] == row_vals[q][2]) return false;

    /* First 2×2 block (columns 0,1) */
    int diag1  = row_vals[p][0] + row_vals[q][1];
    int ant1   = row_vals[p][1] + row_vals[q][0];
    int diff1  = diag1 - ant1;
    if (diff1 < 0) diff1 = -diff1;
    if (diff1 != 6) return false;

    /* Second 2×2 block (columns 1,2) */
    int diag2  = row_vals[p][1] + row_vals[q][2];
    int ant2   = row_vals[p][2] + row_vals[q][1];
    int diff2  = diag2 - ant2;
    if (diff2 < 0) diff2 = -diff2;
    return diff2 == 6;
}

/* Pre‑compute all transition lists */
static void build_transitions(void)
{
    for (int p = 0; p < STATE_COUNT; ++p)
    {
        unsigned short cnt = 0;
        for (int q = 0; q < STATE_COUNT; ++q)
            if (valid_transition(p, q))
                trans_next[p][cnt++] = (short)q;
        trans_cnt[p] = cnt;
    }
}

static void init_tables(void)
{
    if (!tables_inited)
    {
        build_rows();
        build_transitions();
        tables_inited = true;
    }
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset is 1 */

    init_tables();

    static seq_t cur[STATE_COUNT];
    static seq_t nxt[STATE_COUNT];

    /* reset cur to all ones for the first row */
    for (int s = 0; s < STATE_COUNT; ++s)
        cur[s] = (seq_t)1;

    /* perform n transitions (i.e. generate n+1 rows) */
    for (idx_t step = 0; step < n; ++step)
    {
        /* zero next array */
        for (int s = 0; s < STATE_COUNT; ++s)
            nxt[s] = (seq_t)0;

        /* propagate counts from current to next */
        for (int p = 0; p < STATE_COUNT; ++p)
        {
            seq_t val = cur[p];
            if (val == (seq_t)0) continue;

            unsigned short cnt = trans_cnt[p];
            for (unsigned short i = 0; i < cnt; ++i)
            {
                int q = trans_next[p][i];
                nxt[q] += val;
            }
        }

        /* copy next to cur for next iteration */
        for (int s = 0; s < STATE_COUNT; ++s)
            cur[s] = nxt[s];
    }

    /* sum over all final states */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s)
        result += cur[s];

    return result;
}
