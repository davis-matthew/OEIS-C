/*
 * A183548  Number of nX3 0..2 arrays with each element equal to either the
 *          maximum or the minimum of its horizontal and vertical neighbors.
 * https://oeis.org/A183548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays with entries in {0,1,2} such that
 *            every entry equals the minimum or the maximum of its horizontal
 *            and vertical neighbors. The sequence is defined recursively by a
 *            DP over row patterns
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define PARS 27                 /* number of 3‑digit base‑3 patterns */
#define MISSING 27              /* sentinel for a missing neighbour */

/* Pre‑computed 3‑digit values of each pattern. */
static int rowVals[PARS][3];

/* For a given (up,cur) pair, nextCount and nextList give all rows nxt
   for which cur satisfies its condition with neighbours up and nxt. */
static uint8_t nextCount[PARS + 1][PARS];
static int      nextList[PARS + 1][PARS][PARS];

/* We build the tables once. */
static bool initialized = false;

static bool
valid_row(int cur, int up, int down)
{
    /* cur, up, down are pattern indices; use MISSING for missing. */
    const int *c = rowVals[cur];
    int upIdx = (up == MISSING) ? -1 : up;
    int downIdx = (down == MISSING) ? -1 : down;

    for (int j = 0; j < 3; ++j) {
        int val = c[j];
        int mn = 3, mx = -1;       /* neighbours only; values are 0..2 */

        /* left neighbour */
        if (j > 0) {
            int v = rowVals[cur][j - 1];
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
        /* right neighbour */
        if (j < 2) {
            int v = rowVals[cur][j + 1];
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
        /* up neighbour */
        if (upIdx != -1) {
            int v = rowVals[upIdx][j];
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
        /* down neighbour */
        if (downIdx != -1) {
            int v = rowVals[downIdx][j];
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
        /* The cell must be min or max of its neighbours. */
        if (!(val == mn || val == mx))
            return false;
    }
    return true;
}

static void
init_tables(void)
{
    /* Fill rowVals: each pattern is represented by a number 0..26. */
    for (int idx = 0; idx < PARS; ++idx) {
        int x = idx;
        rowVals[idx][0] = x % 3; x /= 3;
        rowVals[idx][1] = x % 3; x /= 3;
        rowVals[idx][2] = x % 3;
    }

    /* Build adjacency lists. */
    for (int up = 0; up <= PARS; ++up) {          /* up==MISSING allowed */
        for (int cur = 0; cur < PARS; ++cur) {
            uint8_t cnt = 0;
            for (int nxt = 0; nxt < PARS; ++nxt) {
                if (valid_row(cur, up, nxt))
                    nextList[up][cur][cnt++] = nxt;
            }
            nextCount[up][cur] = cnt;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, but harmless */
    if (n == 1) return (seq_t)3;      /* only constant rows are allowed */

    if (!initialized)
        init_tables();

    /* DP tables for counts of arrays ending with rows (up, cur). */
    static seq_t dp[PARS][PARS];
    static seq_t newdp[PARS][PARS];

    /* Zero‑initialise dp. */
    for (int i = 0; i < PARS; ++i)
        for (int j = 0; j < PARS; ++j)
            dp[i][j] = (seq_t)0;

    /* Initialise with the first two rows. */
    for (int first = 0; first < PARS; ++first) {
        for (int second = 0; second < PARS; ++second) {
            if (valid_row(first, MISSING, second))
                dp[first][second] = (seq_t)1;
        }
    }

    /* Add rows 3 … n.  Each iteration adds one new row. */
    for (idx_t step = 2; step < n; ++step) {
        /* Zero‑initialize newdp. */
        for (int i = 0; i < PARS; ++i)
            for (int j = 0; j < PARS; ++j)
                newdp[i][j] = (seq_t)0;

        /* Transition. */
        for (int up = 0; up < PARS; ++up) {
            for (int cur = 0; cur < PARS; ++cur) {
                seq_t val = dp[up][cur];
                if (!val)
                    continue;
                uint8_t cnt = nextCount[up][cur];
                for (int k = 0; k < cnt; ++k) {
                    int nxt = nextList[up][cur][k];
                    newdp[cur][nxt] += val;
                }
            }
        }

        /* Move newdp into dp for the next iteration. */
        for (int i = 0; i < PARS; ++i)
            for (int j = 0; j < PARS; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum over final states where the last row satisfies its condition. */
    seq_t ans = (seq_t)0;
    for (int up = 0; up < PARS; ++up) {
        for (int cur = 0; cur < PARS; ++cur) {
            if (dp[up][cur] == 0)
                continue;
            if (valid_row(cur, up, MISSING))
                ans += dp[up][cur];
        }
    }
    return ans;
}
