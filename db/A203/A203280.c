/*
 * A203280  Number of n X 3 binary arrays with every element neighboring
 *          horizontally or vertically both a 0 and a 1.
 * https://oeis.org/A203280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A203280: Number of n × 3 binary arrays where every cell has both a
 * neighboring 0 and a neighboring 1 (horizontally or vertically).
 *
 * Computation by dynamic programming over the rows.  Each pair of
 * consecutive rows is a state, and transitions are pre‑computed for
 * rows that satisfy the adjacency condition for the middle row.
 * The first row is tested against its single down neighbour; the last
 * row is tested against its single up neighbour.
 *
 * Time:  O(8⁴·n) = O(n)  (constant factor 512), negligible for n ≤ 10 000.
 * Memory: O(1)  (just a few small tables).
 */

#include <stdbool.h>

/* Number of distinct 3‑bit rows */
enum { PAT = 8 };

/* Transition tables */
static bool ok_mid[PAT][PAT][PAT];   /* up, cur, down */
static bool ok_first[PAT][PAT];       /* down, top */
static bool ok_last[PAT][PAT];        /* up, bottom */
static bool tables_inited = false;

/* Build the tables once.  The code below replaces the buggy
 * (v?one:zero)=true; statements with proper assignment. */
static void init_tables(void)
{
    if (tables_inited) return;

    /* First row: top row only has a down neighbour. */
    for (int a = 0; a < PAT; ++a)          /* a = down */
        for (int b = 0; b < PAT; ++b) {    /* b = top   */
            bool good_first = true;
            for (int j = 0; j < 3; ++j) {
                bool zero = false, one = false;
                if (j > 0) {
                    int v = (b >> (j-1)) & 1;
                    if (v) one = true; else zero = true;
                }
                if (j < 2) {
                    int v = (b >> (j+1)) & 1;
                    if (v) one = true; else zero = true;
                }
                /* down neighbour */
                int v = (a >> j) & 1;
                if (v) one = true; else zero = true;

                if (!(zero && one)) { good_first = false; break; }
            }
            ok_first[a][b] = good_first;

            /* Last row: bottom row only has an up neighbour. */
            bool good_last = true;
            for (int j = 0; j < 3; ++j) {
                bool zero = false, one = false;
                if (j > 0) {
                    int v = (b >> (j-1)) & 1;
                    if (v) one = true; else zero = true;
                }
                if (j < 2) {
                    int v = (b >> (j+1)) & 1;
                    if (v) one = true; else zero = true;
                }
                /* up neighbour */
                int v = (a >> j) & 1;
                if (v) one = true; else zero = true;

                if (!(zero && one)) { good_last = false; break; }
            }
            ok_last[a][b] = good_last;
        }

    /* Middle row: has both an up and a down neighbour. */
    for (int a = 0; a < PAT; ++a)          /* a = up   */
        for (int b = 0; b < PAT; ++b)      /* b = cur  */
            for (int c = 0; c < PAT; ++c) { /* c = down */
                bool good_mid = true;
                for (int j = 0; j < 3; ++j) {
                    bool zero = false, one = false;
                    if (j > 0) {
                        int v = (b >> (j-1)) & 1;
                        if (v) one = true; else zero = true;
                    }
                    if (j < 2) {
                        int v = (b >> (j+1)) & 1;
                        if (v) one = true; else zero = true;
                    }
                    /* up neighbour */
                    int v = (a >> j) & 1;
                    if (v) one = true; else zero = true;
                    /* down neighbour */
                    v = (c >> j) & 1;
                    if (v) one = true; else zero = true;

                    if (!(zero && one)) { good_mid = false; break; }
                }
                ok_mid[a][b][c] = good_mid;
            }

    tables_inited = true;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* a(1)=0, a(0) unused */

    init_tables();

    /* dp[prev][cur] = number of ways to reach this pair of rows. */
    seq_t dp[PAT][PAT] = {0};
    seq_t ndp[PAT][PAT] = {0};

    /* Initialise with all first two rows that satisfy the first‑row rule. */
    for (int first = 0; first < PAT; ++first)
        for (int second = 0; second < PAT; ++second)
            if (ok_first[second][first])
                dp[first][second] = (seq_t)1;

    /* Process rows 3 … n. */
    for (idx_t row = 3; row <= n; ++row) {
        /* Clear ndp. */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < PAT; ++prev)
            for (int cur = 0; cur < PAT; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == 0) continue;
                for (int nxt = 0; nxt < PAT; ++nxt)
                    if (ok_mid[prev][cur][nxt])
                        ndp[cur][nxt] = ndp[cur][nxt] + ways;
            }

        /* Move ndp to dp for the next iteration. */
        for (int i = 0; i < PAT; ++i)
            for (int j = 0; j < PAT; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Count valid completions where the last row has no down neighbour. */
    seq_t res = 0;
    for (int prev = 0; prev < PAT; ++prev)
        for (int last = 0; last < PAT; ++last)
            if (ok_last[prev][last])
                res = res + dp[prev][last];

    return res;
}
