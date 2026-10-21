/*
 * A230690  Number of (n+2)X(1+2) 0..2 arrays x(i,j) with each element
 *          horizontally, vertically, diagonally or antidiagonally next to at
 *          least two elements with value (x(i,j)+1) mod 3, and upper left
 *          element zero.
 * https://oeis.org/A230690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Computes A230690 (Number of (n+2)×3 arrays with the given adjacency
 *  property and top‑left zero).  The implementation follows the
 *  definition: for each n we perform a dynamic‑programming traversal
 *  over the rows, keeping the last two rows as state.  All transitions
 *  are pre‑computed once.  No hard‑coded terms are used.
 */

#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* Pre‑computed representation of all 3‑cell rows (base‑3 digits). */
static int pat[27][3];

/* Transition table: does the middle row satisfy the condition
 * when surrounded by 'above' and 'below' rows ? */
static bool ok_mid[27][27][27];

/* ------------------------------------------------------------------- */
/* Helper that checks whether a middle row `mid` satisfies the
   adjacency requirement given the rows `above` and `below`.
   `above` or `below` may be -1 meaning that the neighbor does not
   exist (outside the grid). */
static bool check_row(int above, int mid, int below)
{
    for (int col = 0; col < 3; ++col) {
        int val   = pat[mid][col];
        int need  = (val + 1) % 3;
        int cnt   = 0;

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int c = col + dc;
                if (c < 0 || c >= 3) continue;

                int neigh;
                if (dr == -1) {            /* above */
                    if (above < 0) continue;
                    neigh = pat[above][c];
                } else if (dr == 1) {      /* below */
                    if (below < 0) continue;
                    neigh = pat[below][c];
                } else {                   /* same row */
                    neigh = pat[mid][c];
                }

                if (neigh == need) ++cnt;
            }
        }
        if (cnt < 2) return false;        /* condition violated */
    }
    return true;                           /* all cells satisfied */
}

/* ------------------------------------------------------------------- */
/* Pre‑initialise the pattern table and transition table the first
   time a() is called. */
static bool data_inited = false;

static void init_data(void)
{
    /* build all 3‑cell patterns (0..26) */
    for (int p = 0; p < 27; ++p) {
        int x = p;
        for (int c = 0; c < 3; ++c) {
            pat[p][c] = x % 3;
            x /= 3;
        }
    }

    /* compute ok_mid table */
    for (int a = 0; a < 27; ++a)
        for (int m = 0; m < 27; ++m)
            for (int b = 0; b < 27; ++b)
                ok_mid[a][m][b] = check_row(a, m, b);

    data_inited = true;
}

/* ------------------------------------------------------------------- */
/* Compute a(n) by dynamic programming over rows. */
static seq_t dp_compute(idx_t n)
{
    idx_t rows = n + 2;               /* number of rows in the grid   */
    /* dp and next arrays: dp[prev_row][curr_row] = number of ways
       to reach this state after processing the current row. */
    static seq_t dp[27][27];
    static seq_t next[27][27];

    /* clear dp */
    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j)
            dp[i][j] = 0;

    /* initialise first two rows */
    for (int r0 = 0; r0 < 27; ++r0) {
        if (pat[r0][0] != 0) continue;          /* top‑left fixed to 0 */
        for (int r1 = 0; r1 < 27; ++r1) {
            if (check_row(-1, r0, r1))           /* no row above      */
                dp[r0][r1] = 1;
        }
    }

    /* process rows 2 .. rows-1 */
    for (idx_t r = 2; r < rows; ++r) {
        /* clear next */
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                next[i][j] = 0;

        for (int p0 = 0; p0 < 27; ++p0) {
            for (int p1 = 0; p1 < 27; ++p1) {
                seq_t ways = dp[p0][p1];
                if (!ways) continue;
                for (int cur = 0; cur < 27; ++cur) {
                    if (!ok_mid[p0][p1][cur]) continue;
                    next[p1][cur] += ways;
                }
            }
        }
        /* copy next to dp for the next iteration */
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                dp[i][j] = next[i][j];
    }

    /* sum over states where the last row satisfies the condition
       with no row below. */
    seq_t total = 0;
    for (int p0 = 0; p0 < 27; ++p0) {
        for (int p1 = 0; p1 < 27; ++p1) {
            seq_t ways = dp[p0][p1];
            if (!ways) continue;
            if (check_row(p0, p1, -1))           /* no row below  */
                total += ways;
        }
    }

    return total;
}

/* ------------------------------------------------------------------- */
/* Required external function implementing the sequence. */
seq_t a(idx_t n)
{
    if (!data_inited) init_data();   /* one‑time global initialization */
    return dp_compute(n);             /* compute by definition */
}
