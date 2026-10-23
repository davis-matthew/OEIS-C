/*
 * A183577  Half the number of nX2 0..3 arrays with each element equal to
 *          either the maximum or the minimum of its horizontal and vertical
 *          neighbors.
 * https://oeis.org/A183577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * (number of n×2 arrays of values 0..3 such that
 *            each element equals either the maximum or the minimum of its
 *            horizontal and vertical neighbors). The implementation counts
 *            the arrays by dynamic programming over rows, then divides by two
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#define ROWS 16        /* 4^2 possible row patterns (two columns) */

/* Precomputed row values: left and right column of each pattern */
static int L[ROWS];
static int R[ROWS];

/* Tables of validity of first and last row pairs, and transition
   adjacency list for middle rows */
static unsigned char valid_first[ROWS][ROWS];
static unsigned char valid_last [ROWS][ROWS];
static unsigned char adjcnt[ROWS * ROWS];
static unsigned char adj[ROWS * ROWS][ROWS];

/* Helper to test if a cell satisfies the max / min condition with
   the given neighbors. */
static inline int cell_ok(int cur, int other, int up, int down,
                          int has_up, int has_down)
{
    int mx = -1, mn = 4;
    if (other < mn) mn = other;
    if (other > mx) mx = other;
    if (has_up) { if (up < mn) mn = up; if (up > mx) mx = up; }
    if (has_down){ if (down < mn) mn = down; if (down > mx) mx = down; }
    return (cur == mx) || (cur == mn);
}

/* Precompute tables (executed once) */
static void init(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    /* Fill row value tables */
    for (int p = 0; p < ROWS; ++p) {
        L[p] = p >> 2;     /* high 2 bits */
        R[p] = p & 3;      /* low 2 bits */
    }

    /* First row vs second row: only down neighbor, no up */
    for (int p = 0; p < ROWS; ++p)
        for (int q = 0; q < ROWS; ++q) {
            int ok0 = cell_ok(L[p], R[p], 0, L[q], 0, 1);
            int ok1 = cell_ok(R[p], L[p], 0, R[q], 0, 1);
            valid_first[p][q] = ok0 && ok1;
        }

    /* Last row vs previous row: only up neighbor, no down */
    for (int p = 0; p < ROWS; ++p)
        for (int q = 0; q < ROWS; ++q) {
            int ok0 = cell_ok(L[q], R[q], L[p], 0, 1, 0);
            int ok1 = cell_ok(R[q], L[q], R[p], 0, 1, 0);
            valid_last[p][q] = ok0 && ok1;
        }

    /* Transitions: middle rows with both up and down neighbors */
    for (int prev = 0; prev < ROWS; ++prev)
        for (int curr = 0; curr < ROWS; ++curr) {
            int s = (prev << 4) | curr;
            int cnt = 0;
            for (int next = 0; next < ROWS; ++next) {
                int ok0 = cell_ok(L[curr], R[curr], L[prev], L[next], 1, 1);
                int ok1 = cell_ok(R[curr], L[curr], R[prev], R[next], 1, 1);
                if (ok0 && ok1) {
                    adj[s][cnt++] = next;
                }
            }
            adjcnt[s] = cnt;
        }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, just return 0 */

    if (n == 1) return 2;          /* 4 assignments / 2 */

    init();                       /* ensure tables are ready */

    /* DP arrays: dp[prev][curr] = number of ways to reach state (prev,curr) */
    static seq_t dp[ROWS][ROWS];
    static seq_t newdp[ROWS][ROWS];
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            dp[i][j] = 0;

    /* Initialize with all valid first pairs */
    for (int p = 0; p < ROWS; ++p)
        for (int q = 0; q < ROWS; ++q)
            if (valid_first[p][q])
                dp[p][q] = 1;

    /* Process rows 3 .. n (if any) */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < ROWS; ++prev)
            for (int curr = 0; curr < ROWS; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val) continue;
                int s = (prev << 4) | curr;
                int cnt = adjcnt[s];
                for (int k = 0; k < cnt; ++k) {
                    int nxt = adj[s][k];
                    newdp[curr][nxt] += val;
                }
            }

        /* swap dp and newdp */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum over final states that satisfy bottom row condition */
    seq_t total = 0;
    for (int p = 0; p < ROWS; ++p)
        for (int q = 0; q < ROWS; ++q)
            if (dp[p][q] && valid_last[p][q])
                total += dp[p][q];

    /* The sequence definition is half the number of arrays */
    return total >> 1;
}
