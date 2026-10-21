/*
 * A236879  Number of (n+1) X (3+1) 0..2 arrays with the maximum plus the
 *          upper median plus the lower median of every 2 X 2 subblock equal.
 * https://oeis.org/A236879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A236879: number of (n+1)×4 arrays over {0,1,2} such that
   for every 2×2 subblock the sum of the top three entries is the same
   across all subblocks.  For each possible common sum S (0 … 6) we
   pre‑compute the adjacency lists of rows that can follow each other
   while keeping the sum of the top three entries equal to S.  Then the
   number of valid arrays of length n+1 equals the number of walks of
   length n in the resulting directed graph.  The total count is the
   sum of walk counts over all S. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define NUM_ROWS 81   /* 3^4 possible 4‑tuples of {0,1,2} */
#define NUM_SUMS 7    /* possible sums of the top three entries: 0 … 6 */

static uint8_t row_val[NUM_ROWS][4];   /* decomposed row digits 0..2 */
static uint8_t adj[NUM_SUMS][NUM_ROWS][NUM_ROWS]; /* adjacency lists (flattened) */
static uint8_t deg[NUM_SUMS][NUM_ROWS];           /* degree per row per sum */
static bool built = false;

/* pre‑compute row digit tables */
static void precompute_rows(void)
{
    for (int r = 0; r < NUM_ROWS; ++r) {
        int v = r;
        for (int c = 0; c < 4; ++c) {
            row_val[r][c] = (uint8_t)(v % 3);
            v /= 3;
        }
    }
}

/* build adjacency lists for all sums */
static void build(void)
{
    if (built) return;
    precompute_rows();

    for (int s = 0; s < NUM_SUMS; ++s) {
        for (int i = 0; i < NUM_ROWS; ++i) {
            uint8_t cnt = 0;
            for (int j = 0; j < NUM_ROWS; ++j) {
                bool ok = true;
                for (int col = 0; col < 3 && ok; ++col) {
                    int a = row_val[i][col];
                    int b = row_val[i][col + 1];
                    int c = row_val[j][col];
                    int d = row_val[j][col + 1];
                    int mn = a;
                    if (b < mn) mn = b;
                    if (c < mn) mn = c;
                    if (d < mn) mn = d;
                    int sum = a + b + c + d - mn;   /* sum of top three */
                    if (sum != s) ok = false;
                }
                if (ok) {
                    adj[s][i][cnt++] = (uint8_t)j;
                }
            }
            deg[s][i] = cnt;
        }
    }
    built = true;
}

/* Count walks of length n in the graph for a given sum s */
static seq_t count_walks(idx_t n, int s)
{
    /* dp vectors */
    seq_t dp_prev[NUM_ROWS];
    seq_t dp_cur[NUM_ROWS];
    for (int r = 0; r < NUM_ROWS; ++r) {
        dp_prev[r] = (seq_t)1;   /* start from any row */
        dp_cur[r]   = (seq_t)0;
    }

    for (size_t step = 0; step < n; ++step) {
        /* zero dp_cur */
        for (int r = 0; r < NUM_ROWS; ++r) dp_cur[r] = (seq_t)0;

        for (int r = 0; r < NUM_ROWS; ++r) {
            seq_t val = dp_prev[r];
            if (val == (seq_t)0) continue;
            uint8_t d = deg[s][r];
            for (uint8_t k = 0; k < d; ++k) {
                int nxt = adj[s][r][k];
                dp_cur[nxt] += val;
            }
        }

        /* swap dp_prev and dp_cur */
        for (int r = 0; r < NUM_ROWS; ++r) {
            dp_prev[r] = dp_cur[r];
        }
    }

    /* sum all end states */
    seq_t total = (seq_t)0;
    for (int r = 0; r < NUM_ROWS; ++r) {
        total += dp_prev[r];
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    build();

    seq_t result = (seq_t)0;
    for (int s = 0; s < NUM_SUMS; ++s) {
        result += count_walks(n, s);
    }
    return result;
}
