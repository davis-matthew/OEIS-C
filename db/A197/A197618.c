/*
 * A197618  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 0,3,1,1,1 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) – number of n×3 arrays over {0,1,2,3,4} where
    each cell with value x has exactly the following number of
    neighbors (horizontal or vertical) equal to a target value:
       0 → 0 neighbors equal to 0
       1 → 1 neighbor equal to 3
       2 → 2 neighbors equal to 1
       3 → 3 neighbors equal to 1
       4 → 4 neighbors equal to 1
    (The array is considered without wrap‑around; border cells
    simply have fewer neighbors.)
    The sequence is computed by a linear DP over row pairs.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Parameters of the problem */
#define NROWS        125          /* 5^3 possible rows */
#define MAXSTATES   (NROWS*NROWS) /* pairs of consecutive rows */

static bool       _initialized = false;
static seq_t      *_dp          = NULL; /* current DP table   */
static seq_t      *_dp_next     = NULL; /* next DP table      */
static int        *_next_offset = NULL; /* prefix sums of adj.   */
static int        *_next_list   = NULL; /* flat adjacency list   */

/* Pre‑computed values of each row */
static uint8_t     _row_vals[NROWS][3];

/* For each value x: number of required neighbors
   equal to target value _tgt[x]                 */
static const int   _need[5]   = {0, 1, 2, 3, 4};
static const int   _tgt [5]   = {0, 3, 1, 1, 1};

/* Test whether a middle row satisfies its constraints
   given up and down rows (-1 for none). */
static bool _check_rows(int up, int mid, int down)
{
    const uint8_t *mv = _row_vals[mid];
    for (int c = 0; c < 3; ++c) {
        int v    = mv[c];
        int req  = _need[v];
        int tgt  = _tgt [v];
        int cnt  = 0;
        if (c > 0   && mv[c-1] == tgt) ++cnt;
        if (c < 2   && mv[c+1] == tgt) ++cnt;
        if (up   >= 0 && _row_vals[up][c] == tgt) ++cnt;
        if (down >= 0 && _row_vals[down][c] == tgt) ++cnt;
        if (cnt != req) return false;
    }
    return true;
}

/* Initialise _row_vals with the 3‑digit base‑5 representation of each row. */
static void _init_rows(void)
{
    for (int idx = 0; idx < NROWS; ++idx) {
        int tmp = idx;
        for (int c = 0; c < 3; ++c) {
            _row_vals[idx][c] = tmp % 5;
            tmp /= 5;
        }
    }
}

/* Build adjacency tables and DP tables. */
static void _build_tables(void)
{
    _init_rows();

    /* Count how many next rows each (a,b) pair admits. */
    int *cnts = (int *)malloc(sizeof(int) * MAXSTATES);
    int total = 0;
    for (int a = 0; a < NROWS; ++a) {
        for (int b = 0; b < NROWS; ++b) {
            int idx  = a * NROWS + b;
            int cnt  = 0;
            for (int c = 0; c < NROWS; ++c) {
                if (_check_rows(a, b, c)) ++cnt;
            }
            cnts[idx] = cnt;
            total += cnt;
        }
    }

    /* Allocate flat adjacency array and offsets. */
    _next_offset = (int *)malloc(sizeof(int) * (MAXSTATES + 1));
    _next_list   = (int *)malloc(sizeof(int) * total);
    _next_offset[0] = 0;
    for (int i = 0; i < MAXSTATES; ++i)
        _next_offset[i+1] = _next_offset[i] + cnts[i];

    /* Fill adjacency list. */
    int *pos = (int *)malloc(sizeof(int) * MAXSTATES);
    for (int i = 0; i < MAXSTATES; ++i)
        pos[i] = 0;
    for (int a = 0; a < NROWS; ++a) {
        for (int b = 0; b < NROWS; ++b) {
            int idx  = a * NROWS + b;
            int base = _next_offset[idx];
            for (int c = 0; c < NROWS; ++c) {
                if (_check_rows(a, b, c))
                    _next_list[base + pos[idx]++] = c;
            }
        }
    }
    free(cnts);
    free(pos);

    /* Allocate DP tables. */
    _dp       = (seq_t *)calloc(MAXSTATES, sizeof(seq_t));
    _dp_next  = (seq_t *)calloc(MAXSTATES, sizeof(seq_t));

    _initialized = true;
}

seq_t a(idx_t n)
{
    if (!_initialized) _build_tables();

    /* The sequence is 1‑based. */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < NROWS; ++r)
            if (_check_rows(-1, r, -1))
                ans += 1;
        return ans;
    }

    /* Initialise DP for the first two rows. */
    for (int i = 0; i < MAXSTATES; ++i) _dp[i] = 0;
    for (int r1 = 0; r1 < NROWS; ++r1)
        for (int r2 = 0; r2 < NROWS; ++r2)
            if (_check_rows(-1, r1, r2))
                _dp[r1 * NROWS + r2] = 1;

    if (n == 2) {
        seq_t ans = 0;
        for (int idx = 0; idx < MAXSTATES; ++idx) {
            seq_t cur = _dp[idx];
            if (!cur) continue;
            int r1 = idx / NROWS;
            int r2 = idx % NROWS;
            if (_check_rows(r1, r2, -1))
                ans += cur;
        }
        return ans;
    }

    /* DP for rows 3 … n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* Zero next table. */
        for (int i = 0; i < MAXSTATES; ++i)
            _dp_next[i] = 0;

        for (int idx = 0; idx < MAXSTATES; ++idx) {
            seq_t cur = _dp[idx];
            if (!cur) continue;
            int start = _next_offset[idx];
            int end   = _next_offset[idx+1];
            for (int k = start; k < end; ++k) {
                int r3 = _next_list[k];
                int nidx = (idx % NROWS) * NROWS + r3;
                _dp_next[nidx] += cur;
            }
        }
        /* Swap tables. */
        seq_t *tmp = _dp;
        _dp = _dp_next;
        _dp_next = tmp;
    }

    /* Sum over final states where the bottom row satisfies constraints. */
    seq_t ans = 0;
    for (int idx = 0; idx < MAXSTATES; ++idx) {
        seq_t cur = _dp[idx];
        if (!cur) continue;
        int r1 = idx / NROWS;
        int r2 = idx % NROWS;
        if (_check_rows(r1, r2, -1))
            ans += cur;
    }
    return ans;
}
