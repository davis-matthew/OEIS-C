/*
 * A238317  Number of (n+1)X(2+1) 0..2 arrays with no element greater than all
 *          horizontal neighbors or less than all vertical neighbors.
 * https://oeis.org/A238317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by the recurrence
 *            a(n)=6*a(n-1)-12*a(n-2)+39*a(n-3)+6*a(n-4)+62*a(n-5)+49*a(n-6)+33*a(n-7)
 *            -32*a(n-8)-41*a(n-9)-12*a(n-10)+6*a(n-11)+5*a(n-12)+a(n-13);
 *            Initial values:
 *            a(1)=9,a(2)=67,a(3)=376,a(4)=1867,a(5)=9489,a(6)=50232,a(7)=267174
 *            a(8)=1408341,a(9)=7395987,a(10)=38886701. (full d...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

enum { ROWS = 27 };

static bool initialized = false;

/* All 3‑tuples of values 0..2 */
static uint8_t rows[ROWS][3];
/* Whether a tuple satisfies the horizontal constraints */
static bool row_valid[ROWS];
/* Compatibility of two rows for top/bottom boundaries */
static bool top_compat[ROWS][ROWS];

/* For each pair (prev,curr) store all next rows that satisfy the vertical constraint
   for curr: curr[col] >= min(prev[col], next[col])  for all col=0..2. */
static int next_list[ROWS][ROWS][ROWS];  /* next_list[prev][curr][k] = next row index */
static int next_count[ROWS][ROWS];       /* number of next rows for each pair */

/* Pre‑compute the tables once. */
static void precompute(void)
{
    if (initialized) return;

    /* Generate all 27 rows and test horizontal constraints. */
    for (int a = 0; a <= 2; ++a)
        for (int b = 0; b <= 2; ++b)
            for (int c = 0; c <= 2; ++c) {
                int idx = a * 9 + b * 3 + c;
                rows[idx][0] = (uint8_t)a;
                rows[idx][1] = (uint8_t)b;
                rows[idx][2] = (uint8_t)c;
                /* Horizontal: left <= mid && right <= mid && mid not greater than both */
                bool ok = (a <= b) && (c <= b) && !(b > a && b > c);
                row_valid[idx] = ok;
            }

    /* Top/bottom compatibility: first row >= second row (for top boundary),
       second row >= first row (for bottom boundary).  Here we store
       prev >= curr for reuse in the DP, and use reversed check later. */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j) {
            bool ok = true;
            if (row_valid[i] && row_valid[j]) {
                for (int k = 0; k < 3; ++k)
                    if (rows[i][k] < rows[j][k]) { ok = false; break; }
            } else ok = false;
            top_compat[i][j] = ok;
        }

    /* Pre‑compute adjacency list for the vertical constraint. */
    for (int p = 0; p < ROWS; ++p) if (row_valid[p])
        for (int c = 0; c < ROWS; ++c) if (row_valid[c]) {
            int cnt = 0;
            for (int n = 0; n < ROWS; ++n) if (row_valid[n]) {
                bool ok = true;
                for (int k = 0; k < 3; ++k) {
                    int mn = (rows[p][k] < rows[n][k]) ? rows[p][k] : rows[n][k];
                    if (rows[c][k] < mn) { ok = false; break; }
                }
                if (ok) next_list[p][c][cnt++] = n;
            }
            next_count[p][c] = cnt;
        }

    initialized = true;
}

/* Count the number of valid (n+1) × 3 arrays.  idx_t is unsigned 64‑bit,
   seq_t is the chosen integer type (uint8_t, uint16_t, …, unsigned __int128
   or arbitrary precision). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 (n≥1) */

    precompute();                  /* ensure tables are ready */

    /* DP[prev][curr] = number of ways to reach current pair of rows. */
    seq_t dp[ROWS][ROWS];
    seq_t newdp[ROWS][ROWS];

    /* Initialize DP with the first two rows (top boundary). */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            dp[i][j] = 0;

    for (int i = 0; i < ROWS; ++i)
        if (row_valid[i])
            for (int j = 0; j < ROWS; ++j)
                if (row_valid[j] && top_compat[i][j])
                    dp[i][j] = 1;

    /* Add rows until we have n+1 rows in total. */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero newdp */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < ROWS; ++prev)
            if (row_valid[prev])
                for (int curr = 0; curr < ROWS; ++curr)
                    if (row_valid[curr]) {
                        seq_t val = dp[prev][curr];
                        if (val == 0) continue;
                        int cnt = next_count[prev][curr];
                        for (int t = 0; t < cnt; ++t) {
                            int nxt = next_list[prev][curr][t];
                            newdp[curr][nxt] += val;
                        }
                    }

        /* copy newdp to dp for the next iteration */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Bottom boundary: last row must be >= previous row in each column. */
    seq_t ans = 0;
    for (int prev = 0; prev < ROWS; ++prev)
        if (row_valid[prev])
            for (int last = 0; last < ROWS; ++last)
                if (row_valid[last]) {
                    bool ok = true;
                    for (int k = 0; k < 3; ++k)
                        if (rows[prev][k] > rows[last][k]) { ok = false; break; }
                    if (ok) ans += dp[prev][last];
                }

    return ans;
}
