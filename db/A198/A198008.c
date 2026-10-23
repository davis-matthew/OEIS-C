/*
 * A198008  Number of n X 3 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,3,0,1,2
 *          for x=0,1,2,3,4.
 * https://oeis.org/A198008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A198008: number of n×3 0..4 arrays with each element x equal to the
   number of its horizontal and vertical neighbours that equal
   3-(x==0?0:1:2:3:4) for x=0,1,2,3,4 respectively.  |
   Recurrence: none – we compute by a linear‑time DP on the rows.   |
   Initial values: a(1)=2, a(2)=12, a(3)=51, … (see OEIS entry).     |
   Implementation: dynamic programming over the 125 possible rows of the
   3‑column array.                                     */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static const int R[5] = {3, 3, 0, 1, 2};

#define ROWS 125   /* 5³ possible rows 0..124 */
#define COLS 3

/* Precomputed tables for a quickly rejected row in a given context. */
static int  row_vals[ROWS][COLS];
static int  lr_count[ROWS][COLS];
static unsigned char up_match[ROWS][ROWS][COLS];
static bool start_self[ROWS];
static bool start_ok[ROWS][ROWS];
static unsigned char next_len[ROWS * ROWS];
static int next_rows[ROWS * ROWS][ROWS];

/* Check whether a given row satisfies the neighbour constraints
   with an optional row above (up) and/or below (down). */
static bool
check_row(int row, int up, int down)
{
    for (int j = 0; j < COLS; ++j)
    {
        int cnt = lr_count[row][j];
        if (up   >= 0) cnt += up_match[row][up][j];
        if (down >= 0) cnt += up_match[row][down][j];
        if (cnt != row_vals[row][j]) return false;
    }
    return true;
}

/* Build all tables once.  Called lazily from a(). */
static void
init_tables(void)
{
    /* Decode each row from base‑5 representation. */
    for (int r = 0; r < ROWS; ++r)
    {
        int t = r;
        for (int j = 0; j < COLS; ++j)
        {
            row_vals[r][j] = t % 5;
            t /= 5;
        }
    }

    /* Count horizontal neighbours that satisfy the rule. */
    for (int r = 0; r < ROWS; ++r)
        for (int j = 0; j < COLS; ++j)
        {
            int cnt = 0;
            if (j > 0 && row_vals[r][j - 1] == R[row_vals[r][j]]) ++cnt;
            if (j < COLS - 1 && row_vals[r][j + 1] == R[row_vals[r][j]]) ++cnt;
            lr_count[r][j] = cnt;
        }

    /* up_match[a][b][j] == 1 if row b at column j is a valid neighbour
       directly below a. */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            for (int j = 0; j < COLS; ++j)
                up_match[a][b][j] = (row_vals[b][j] == R[row_vals[a][j]]) ? 1 : 0;

    /* First row must have no neighbours above, so check_row(...,-1,-1). */
    for (int r = 0; r < ROWS; ++r) start_self[r] = check_row(r, -1, -1);

    /* Transition from an empty top row to two rows; second row may have
       the first row above it. */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            start_ok[a][b] = check_row(a, -1, b);

    /* Precompute allowable third rows for every pair (a,b). */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
        {
            size_t idx = (size_t)a * ROWS + b;
            unsigned char k = 0;
            for (int c = 0; c < ROWS; ++c)
                if (check_row(b, a, c))
                {
                    next_rows[idx][k++] = c;
                }
            next_len[idx] = k;
        }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    static bool initialized = false;
    if (!initialized)
    {
        init_tables();
        initialized = true;
    }

    /* Handle n==1 separately (a single row, no neighbours). */
    if (n == 1)
    {
        seq_t ans = 0;
        for (int r = 0; r < ROWS; ++r)
            if (start_self[r]) ++ans;
        return ans;
    }

    const size_t states = (size_t)ROWS * (size_t)ROWS;
    seq_t *cur = (seq_t *)calloc(states, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(states, sizeof(seq_t));
    if (!cur || !nxt)
    {
        free(cur);
        free(nxt);
        return 0;
    }

    /* Initialize for the first two rows. */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            if (start_ok[a][b]) cur[(size_t)a * ROWS + b] = 1;

    /* DP over remaining rows (from row 3 to row n). */
    for (idx_t step = 2; step < n; ++step)
    {
        memset(nxt, 0, states * sizeof(seq_t));
        for (int a = 0; a < ROWS; ++a)
            for (int b = 0; b < ROWS; ++b)
            {
                seq_t val = cur[(size_t)a * ROWS + b];
                if (!val) continue;
                size_t idx = (size_t)a * ROWS + b;
                unsigned char len = next_len[idx];
                int *arr = next_rows[idx];
                for (unsigned char k = 0; k < len; ++k)
                {
                    int c = arr[k];
                    nxt[(size_t)b * ROWS + c] += val;
                }
            }
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum solutions where the bottom row has no neighbour below. */
    seq_t ans = 0;
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
        {
            seq_t val = cur[(size_t)a * ROWS + b];
            if (!val) continue;
            if (check_row(b, a, -1)) ans += val;
        }

    free(cur);
    free(nxt);
    return ans;
}
