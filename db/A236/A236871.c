/*
 * A236871  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the upper
 *          median minus the lower median of every 2X2 subblock equal.
 * https://oeis.org/A236871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A236871 – Number of (n+1)×5 arrays with entries 0..2 such that for every
 *  2×2 subblock the value “maximum + upper‑median – lower‑median” is the same.
 *  The algorithm builds, for each possible value e∈{0,…,4}, an undirected
 *  graph whose vertices are the 3^5 = 243 possible rows.  Two rows are
 *  adjacent if all 2×2 blocks formed with them have the same e.  For a
 *  fixed e the number of valid arrays equals the number of walks of
 *  length n in that graph; the total is the sum over all e.
 */

#include <stdint.h>
#include <stdbool.h>

#define ROWS 243   /* 3^5 */

/* precomputed data structures */
static int   neighbors[5][ROWS][ROWS];   /* neighbors[e][i][k] */
static int   deg[5][ROWS];              /* number of neighbors for node i in graph e */
static int   e_table[81];               /* e value for a 2×2 block (base‑3 index) */
static bool  table_inited = false;

/* compute the e_table that maps a 4‑cell block (a,b,c,d) represented in
 * base‑3 to the value e = max + upper‑median – lower‑median. */
static void init_e_table(void)
{
    if (table_inited) return;
    for (int idx = 0; idx < 81; ++idx) {
        int t = idx;
        int a = t % 3; t /= 3;
        int b = t % 3; t /= 3;
        int c = t % 3; t /= 3;
        int d = t;
        int v[4] = {a, b, c, d};
        /* simple bubble sort (4 elements) */
        for (int p = 0; p < 4; ++p)
            for (int q = p + 1; q < 4; ++q)
                if (v[p] > v[q]) {
                    int tmp = v[p]; v[p] = v[q]; v[q] = tmp;
                }
        int maxv = v[3];
        int upper = v[2];
        int lower = v[1];
        e_table[idx] = maxv + upper - lower;      /* value in 0..4 */
    }
    table_inited = true;
}

/* initialise the adjacency lists for the 5 graphs.  The result is stored
 * in the static arrays neighbors and deg. */
static void init_adjacency(void)
{
    static bool ready = false;
    if (ready) return;

    init_e_table();

    /* pre‑compute the digits of every possible row */
    static int rowdigits[ROWS][5];
    for (int r = 0; r < ROWS; ++r) {
        int v = r;
        for (int k = 4; k >= 0; --k) {
            rowdigits[r][k] = v % 3;
            v /= 3;
        }
    }

    /* clear degrees */
    for (int e = 0; e < 5; ++e)
        for (int r = 0; r < ROWS; ++r)
            deg[e][r] = 0;

    /* for every unordered pair (i,j) compute the shared e if it exists,
     * then add the edge to the adjacency lists for both directions. */
    for (int i = 0; i < ROWS; ++i) {
        for (int j = i; j < ROWS; ++j) {
            int e_common = -1;
            bool ok = true;
            for (int col = 0; col < 4; ++col) {
                int a = rowdigits[i][col];
                int b = rowdigits[i][col + 1];
                int c = rowdigits[j][col];
                int d = rowdigits[j][col + 1];
                int idx = a * 27 + b * 9 + c * 3 + d;
                int val = e_table[idx];
                if (e_common == -1)
                    e_common = val;
                else if (val != e_common) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;

            if (i == j) {          /* self‑loop */
                deg[e_common][i] += 1;
                neighbors[e_common][i][deg[e_common][i] - 1] = i;
            } else {
                deg[e_common][i] += 1;
                neighbors[e_common][i][deg[e_common][i] - 1] = j;
                deg[e_common][j] += 1;
                neighbors[e_common][j][deg[e_common][j] - 1] = i;
            }
        }
    }

    ready = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_adjacency();

    seq_t total = 0;
    static seq_t dp[ROWS];
    static seq_t ndp[ROWS];

    for (int e = 0; e < 5; ++e) {
        /* start with any first row */
        for (int r = 0; r < ROWS; ++r) dp[r] = 1;

        /* walk of length n (n adjacent row pairs) */
        for (idx_t step = 1; step <= n; ++step) {
            for (int r = 0; r < ROWS; ++r) ndp[r] = 0;
            for (int r = 0; r < ROWS; ++r) {
                if (dp[r] == 0) continue;
                int d = deg[e][r];
                for (int k = 0; k < d; ++k) {
                    int nb = neighbors[e][r][k];
                    ndp[nb] += dp[r];
                }
            }
            for (int r = 0; r < ROWS; ++r) dp[r] = ndp[r];
        }

        for (int r = 0; r < ROWS; ++r) total += dp[r];
    }
    return total;
}
