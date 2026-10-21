/*
 * A203332  Number of nX2 0..5 arrays with every nonzero element less than or
 *          equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A203332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A203332
   by dynamic programming on the two most recent rows.
   Each state is a pair of rows of the n×2 array, encoded
   as (row_{i-1}, row_i).  Transition from (prev,curr) to
   (curr,next) is allowed iff row_i satisfies its constraints
   given its neighbors prev, curr, next.  The first row
   constraints are checked when the second row is added.
   After processing n rows the final row's constraints are
   verified against its single available neighbor. */
#include <stddef.h>

#define MAX_ROWS 36          /* 6^2 possible rows (values 0..5 for two cells) */

static unsigned char rows[MAX_ROWS][2];
static unsigned char first_ok[MAX_ROWS][MAX_ROWS];
static unsigned char next_list[MAX_ROWS][MAX_ROWS][MAX_ROWS];
static unsigned char next_count[MAX_ROWS][MAX_ROWS];
static int tables_initialized = 0;

/* Check whether the row with index rc satisfies its constraints
   when the previous row is rp (or -1 if missing) and the next row is rn (or -1).
   If a neighbor is missing it does not count towards the total. */
static int row_ok(int rc, int rp, int rn)
{
    int a = rows[rc][0];
    int b = rows[rc][1];
    int val, other, up, down, cnt;

    /* cell 0 */
    if (a) {
        cnt = 0;
        if (b >= a) cnt++;
        if (rp >= 0 && rows[rp][0] >= a) cnt++;
        if (rn >= 0 && rows[rn][0] >= a) cnt++;
        if (cnt < 2) return 0;
    }

    /* cell 1 */
    if (b) {
        cnt = 0;
        if (a >= b) cnt++;
        if (rp >= 0 && rows[rp][1] >= b) cnt++;
        if (rn >= 0 && rows[rn][1] >= b) cnt++;
        if (cnt < 2) return 0;
    }

    return 1;
}

static void init_tables(void)
{
    int i, j, k, rc, rp, rn;
    /* Precompute row values */
    for (i = 0; i < MAX_ROWS; ++i) {
        rows[i][0] = i / 6;
        rows[i][1] = i % 6;
    }

    /* First row check: row_rc with previous missing (-1) and next row rn */
    for (rp = 0; rp < MAX_ROWS; ++rp)
        for (rn = 0; rn < MAX_ROWS; ++rn)
            first_ok[rp][rn] = row_ok(rp, -1, rn);

    /* Transition lists: for each (prev, curr), list next rows allowed */
    for (rp = 0; rp < MAX_ROWS; ++rp) {
        for (rc = 0; rc < MAX_ROWS; ++rc) {
            k = 0;
            for (rn = 0; rn < MAX_ROWS; ++rn) {
                if (row_ok(rc, rp, rn))
                    next_list[rp][rc][k++] = (unsigned char)rn;
            }
            next_count[rp][rc] = (unsigned char)k;
        }
    }

    tables_initialized = 1;
}

seq_t a(idx_t n)
{
    seq_t dp[MAX_ROWS][MAX_ROWS];
    seq_t ndp[MAX_ROWS][MAX_ROWS];
    seq_t ans = (seq_t)0;
    int i, j, k, rc, rn;
    int steps, t;

    if (!tables_initialized)
        init_tables();

    if (n == (idx_t)1)
        return (seq_t)1;   /* only the all‑zero row */

    /* Initialize dp[prev][curr] for the first two rows */
    for (i = 0; i < MAX_ROWS; ++i)
        for (j = 0; j < MAX_ROWS; ++j)
            dp[i][j] = (seq_t)0;
    for (rc = 0; rc < MAX_ROWS; ++rc)
        for (rn = 0; rn < MAX_ROWS; ++rn)
            if (first_ok[rc][rn])
                dp[rc][rn] = (seq_t)1;

    steps = (int)(n) - 2;    /* number of transitions after the first two rows */
    for (t = 0; t < steps; ++t) {
        /* zero ndp */
        for (i = 0; i < MAX_ROWS; ++i)
            for (j = 0; j < MAX_ROWS; ++j)
                ndp[i][j] = (seq_t)0;

        for (rc = 0; rc < MAX_ROWS; ++rc)    /* prev row */
            for (rn = 0; rn < MAX_ROWS; ++rn) { /* curr row */
                seq_t val = dp[rc][rn];
                if (val == (seq_t)0)
                    continue;
                k = next_count[rc][rn];
                for (j = 0; j < k; ++j) {
                    int next = next_list[rc][rn][j];
                    ndp[rn][next] += val;
                }
            }
        /* move ndp -> dp */
        for (i = 0; i < MAX_ROWS; ++i)
            for (j = 0; j < MAX_ROWS; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum over states where the final row satisfies its constraints */
    for (rc = 0; rc < MAX_ROWS; ++rc)   /* prev row */
        for (rn = 0; rn < MAX_ROWS; ++rn) { /* curr (final) row */
            seq_t val = dp[rc][rn];
            if (val == (seq_t)0)
                continue;
            if (row_ok(rn, rc, -1))
                ans += val;
        }

    return ans;
}
