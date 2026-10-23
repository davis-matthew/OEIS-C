/*
 * A196450  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,0,2,4,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 2 arrays of values 0..4 such that for each
 *            cell value x, the number of its horizontal and vertical
 *            neighbors equal to mapping[x] equals x. mapping[x] = {3,0,2,4,1}
 *            for x = 0,1,2,3,4
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 56, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define NUM_PAT 25
#define DUMMY 25

/* mapping of each value to the neighbor value that is counted */
static const int mapping[5] = {3, 0, 2, 4, 1};

/* all possible 2‑cell row patterns; pattern index = 5*a + b */
static const int rowVals[NUM_PAT][2] = {
    {0,0},{0,1},{0,2},{0,3},{0,4},
    {1,0},{1,1},{1,2},{1,3},{1,4},
    {2,0},{2,1},{2,2},{2,3},{2,4},
    {3,0},{3,1},{3,2},{3,3},{3,4},
    {4,0},{4,1},{4,2},{4,3},{4,4}
};

/* helper: check that a row satisfies the constraint when its
   up‑ and down‑neighbour rows are given.  DUMMY index means
   that neighbour is missing. */
static bool isValidRow(int cur, int up, int down)
{
    const int *curv = rowVals[cur];
    for (int j = 0; j < 2; ++j) {
        int x = curv[j];
        int target = mapping[x];
        int count = 0;

        /* horizontal neighbours */
        if (j == 1) {            /* has left neighbour */
            if (rowVals[cur][0] == target) count++;
        }
        if (j == 0) {            /* has right neighbour */
            if (rowVals[cur][1] == target) count++;
        }

        /* vertical neighbours */
        if (up != DUMMY) {
            if (rowVals[up][j] == target) count++;
        }
        if (down != DUMMY) {
            if (rowVals[down][j] == target) count++;
        }

        if (count != x) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, but keep safe */

    /* dp[prev][cur] : number of ways for the processed rows
       where 'prev' is the row above the current row and
       'cur' is the current (last) row.  DUMMY=25 means no row. */
    seq_t dp[NUM_PAT + 1][NUM_PAT];
    seq_t ndp[NUM_PAT + 1][NUM_PAT];

    /* initialise: one row so far, no up neighbour */
    for (int i = 0; i <= NUM_PAT; ++i)
        for (int j = 0; j < NUM_PAT; ++j)
            dp[i][j] = 0;
    for (int cur = 0; cur < NUM_PAT; ++cur)
        dp[DUMMY][cur] = 1;

    /* add rows 2 .. n */
    for (idx_t step = 1; step < n; ++step) {
        /* clear ndp */
        for (int i = 0; i <= NUM_PAT; ++i)
            for (int j = 0; j < NUM_PAT; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev <= NUM_PAT; ++prev) {
            for (int cur = 0; cur < NUM_PAT; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == 0) continue;
                for (int nxt = 0; nxt < NUM_PAT; ++nxt) {
                    if (isValidRow(cur, prev, nxt))
                        ndp[cur][nxt] += ways;
                }
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i <= NUM_PAT; ++i)
            for (int j = 0; j < NUM_PAT; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final row has no down neighbour, all rows must be valid */
    seq_t result = 0;
    for (int prev = 0; prev <= NUM_PAT; ++prev) {
        for (int cur = 0; cur < NUM_PAT; ++cur) {
            if (dp[prev][cur] == 0) continue;
            if (isValidRow(cur, prev, DUMMY))
                result += dp[prev][cur];
        }
    }
    return result;
}
