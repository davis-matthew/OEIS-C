/*
 * A236795  Number of (n+1)X(5+1) 0..2 arrays with the maximum plus the upper
 *          median plus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: counts the number of (n+1)×6 arrays over {0,1,2}
   in which every 2×2 subblock satisfies
          max + upper‑median + min = constant.
   Because the condition depends only on pairs of adjacent rows,
   we count the arrays by dynamic programming over rows. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Number of possible rows (3^6) and number of distinct block sums (0–6). */
#define ROWS   729
#define MAX_S  7

/* Pre‑computed row contents: rows_digits[r][i] = digit i of row r (0‑based). */
static unsigned char rows_digits[ROWS][6];

/* Adjacency data structures: for each S and row r,
   store the list of rows r2 that can follow r with that S. */
static unsigned int  *adj_start   = NULL;   /* start index in adj_neighbors */
static unsigned int  *adj_count   = NULL;   /* number of neighbours */
static uint16_t      *adj_neighbors = NULL; /* neighbour list */

/* Helper: compute the sum (max + upper‑median + min) of a 2×2 block formed
   from row1[j], row1[j+1], row2[j], row2[j+1]. */
static int block_sum(const unsigned char *row1,
                     const unsigned char *row2,
                     int j)
{
    unsigned char a = row1[j];
    unsigned char b = row1[j + 1];
    unsigned char c = row2[j];
    unsigned char d = row2[j + 1];
    unsigned char v[4] = {a, b, c, d};
    unsigned char t;

    /*  Sort the four values (insertion sort, very cheap). */
    if (v[0] > v[1]) { t = v[0]; v[0] = v[1]; v[1] = t; }
    if (v[2] > v[3]) { t = v[2]; v[2] = v[3]; v[3] = t; }
    if (v[0] > v[2]) { t = v[0]; v[0] = v[2]; v[2] = t; }
    if (v[1] > v[3]) { t = v[1]; v[1] = v[3]; v[3] = t; }
    if (v[1] > v[2]) { t = v[1]; v[1] = v[2]; v[2] = t; }

    /*  max = v[3], upper‑median = v[2], min = v[0]  */
    return v[0] + v[2] + v[3];
}

/* Build the adjacency tables once.  The tables are indexed by
   idx = S * ROWS + r. */
static void build_graph(void)
{
    static bool graph_built = false;
    if (graph_built) return;

    /* Generate row digits for all 729 rows. */
    for (int r = 0; r < ROWS; ++r) {
        int x = r;
        for (int i = 0; i < 6; ++i) {
            rows_digits[r][i] = (unsigned char)(x % 3);
            x /= 3;
        }
    }

    /* Allocate per‑pair structures. */
    adj_start   = (unsigned int *)malloc(MAX_S * ROWS * sizeof(unsigned int));
    adj_count   = (unsigned int *)malloc(MAX_S * ROWS * sizeof(unsigned int));
    if (!adj_start || !adj_count) exit(EXIT_FAILURE);

    unsigned int total_edges = 0;

    /* First pass: count neighbours for each (S, row). */
    for (int S = 0; S < MAX_S; ++S) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            unsigned int cnt = 0;
            const unsigned char *row1 = rows_digits[r1];
            for (int r2 = 0; r2 < ROWS; ++r2) {
                const unsigned char *row2 = rows_digits[r2];
                bool ok = true;
                for (int j = 0; j < 5; ++j) {
                    if (block_sum(row1, row2, j) != S) { ok = false; break; }
                }
                if (ok) ++cnt;
            }
            unsigned int idx = S * ROWS + r1;
            adj_count[idx] = cnt;
            total_edges += cnt;
        }
    }

    /* Allocate neighbour list. */
    adj_neighbors = (uint16_t *)malloc(total_edges * sizeof(uint16_t));
    if (!adj_neighbors) exit(EXIT_FAILURE);

    /* Second pass: fill neighbour lists. */
    unsigned int offset = 0;
    for (int S = 0; S < MAX_S; ++S) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            unsigned int idx = S * ROWS + r1;
            adj_start[idx] = offset;
            const unsigned char *row1 = rows_digits[r1];
            for (int r2 = 0; r2 < ROWS; ++r2) {
                const unsigned char *row2 = rows_digits[r2];
                bool ok = true;
                for (int j = 0; j < 5; ++j) {
                    if (block_sum(row1, row2, j) != S) { ok = false; break; }
                }
                if (ok) {
                    adj_neighbors[offset++] = (uint16_t)r2;
                }
            }
            /* adj_count[idx] equals offset - adj_start[idx]  */
        }
    }

    graph_built = true;
}

/* Main sequence function.  Uses dp[2][ROWS] and sums over all
   allowed block sums S.  The index n is the number of 2×2 subblock
   transitions, i.e., number of rows minus one. */
seq_t a(idx_t n)
{
    build_graph();                 /* ensure adjacency is ready */

    seq_t dp[2][ROWS] = {{0}};
    int cur = 0, nxt = 1;
    seq_t total = 0;

    for (int S = 0; S < MAX_S; ++S) {
        /* Initial state: any first row is acceptable. */
        for (int r = 0; r < ROWS; ++r) dp[cur][r] = (seq_t)1;

        for (idx_t step = 0; step < n; ++step) {
            for (int r = 0; r < ROWS; ++r) dp[nxt][r] = (seq_t)0;

            for (int r = 0; r < ROWS; ++r) {
                seq_t ways = dp[cur][r];
                if (!ways) continue;

                unsigned int idx = S * ROWS + r;
                unsigned int start  = adj_start[idx];
                unsigned int count  = adj_count[idx];
                for (unsigned int k = 0; k < count; ++k) {
                    int nb = adj_neighbors[start + k];
                    dp[nxt][nb] += ways;
                }
            }
            /* Swap current and next. */
            int tmp = cur; cur = nxt; nxt = tmp;
        }

        /* Sum all possible ending rows for this S. */
        for (int r = 0; r < ROWS; ++r) total += dp[cur][r];
    }

    return total;
}
