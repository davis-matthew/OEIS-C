/*
 * A234078  Number of (n+1)X(3+1) 0..4 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 10.
 * https://oeis.org/A234078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A234078: number of (n+1) × 4 arrays with entries 0..4
   such that every 2×2 subblock has the sum of the absolute values
   of all six edge and diagonal differences equal to 10.
   Computed by a transfer‑matrix method.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Number of all possible rows (4 digits base‑5). */
#define ROWS 625

/* Representation of the 625 rows. */
static int rows[ROWS][4];

/* Adjacency lists: rows i can be followed by rows in adj[i]. */
static int *adj[ROWS];
static int adj_len[ROWS];
static int adj_cap[ROWS];
static bool adj_inited = false;

/* Build adjacency once.  */
static void init_adj(void)
{
    if (adj_inited) return;

    /* Generate all 4‑digit base‑5 rows. */
    for (int i = 0; i < ROWS; ++i) {
        int t = i;
        for (int c = 3; c >= 0; --c) {
            rows[i][c] = t % 5;
            t /= 5;
        }
    }

    /* Allocate adjacency storage. */
    for (int i = 0; i < ROWS; ++i) {
        adj_len[i] = 0;
        adj_cap[i] = 4;
        adj[i] = (int*)malloc(adj_cap[i] * sizeof(int));
    }

    /* Build compatibility edges. */
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            bool ok = true;
            for (int col = 0; col < 3; ++col) {
                int a0 = rows[i][col];
                int a1 = rows[i][col + 1];
                int b0 = rows[j][col];
                int b1 = rows[j][col + 1];
                int sum =
                    abs(a0 - a1) + abs(a1 - b1) + abs(b1 - b0) +
                    abs(b0 - a0) + abs(a0 - b1) + abs(a1 - b0);
                if (sum != 10) { ok = false; break; }
            }
            if (ok) {
                if (adj_len[i] == adj_cap[i]) {
                    adj_cap[i] *= 2;
                    adj[i] = (int*)realloc(adj[i], adj_cap[i] * sizeof(int));
                }
                adj[i][adj_len[i]++] = j;
            }
        }
    }

    adj_inited = true;
}

/* The main sequence function.  */
seq_t a(idx_t n)
{
    init_adj();
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    static seq_t dp[ROWS];
    static seq_t next[ROWS];

    /* Initial vector: one way to start at any row. */
    for (int i = 0; i < ROWS; ++i) dp[i] = (seq_t)1;

    /* Perform n transitions (n steps). */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < ROWS; ++i) next[i] = (seq_t)0;
        for (int i = 0; i < ROWS; ++i) {
            if (dp[i] == (seq_t)0) continue;
            int *dest = adj[i];
            int len   = adj_len[i];
            for (int k = 0; k < len; ++k) {
                int j = dest[k];
                next[j] += dp[i];
            }
        }
        for (int i = 0; i < ROWS; ++i) dp[i] = next[i];
    }

    /* Sum over all possible final rows. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < ROWS; ++i) total += dp[i];
    return total;
}
