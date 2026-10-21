/*
 * A234330  Number of (n+1) X (5+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by dynamic programming over rows for the (n+1)×6 arrays
   with entries 0…3 in which every 2×2 subblock has its diagonal sum
   differing from its antidiagonal sum by ±1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define S 4096   /* number of possible rows: 4^6 */

/* Lookup table: state_vals[r][k] is the k‑th digit of row r (0..5). */
static uint8_t state_vals[S][6];

/* Adjacency lists: adj[i] points to an array of row indices that can
   follow row i.  adj_len[i] stores the number of such neighbours. */
static int **adj;
static size_t *adj_len;

/* Pre‑compute all rows and their allowed transitions. */
static void init_precompute(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Decode all rows into their 6 digits (base 4). */
    for (int r = 0; r < S; ++r) {
        uint16_t x = (uint16_t)r;
        for (int k = 0; k < 6; ++k) {
            state_vals[r][k] = (uint8_t)(x & 3);
            x >>= 2;
        }
    }

    /* First pass: count edges to allocate adjacency arrays. */
    adj_len = (size_t*)calloc(S, sizeof(size_t));
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            bool ok = true;
            for (int k = 0; k < 5; ++k) {
                int a = state_vals[i][k];
                int b = state_vals[i][k + 1];
                int c = state_vals[j][k];
                int d = state_vals[j][k + 1];
                int diff = (a + d) - (b + c);
                if (diff != 1 && diff != -1) { ok = false; break; }
            }
            if (ok) adj_len[i]++;
        }
    }

    /* Allocate adjacency lists. */
    adj = (int**)malloc(S * sizeof(int*));
    for (int i = 0; i < S; ++i) {
        if (adj_len[i] == 0) {
            adj[i] = NULL;
        } else {
            adj[i] = (int*)malloc(adj_len[i] * sizeof(int));
        }
    }

    /* Second pass: fill adjacency lists. */
    size_t *adj_pos = (size_t*)calloc(S, sizeof(size_t));
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            bool ok = true;
            for (int k = 0; k < 5; ++k) {
                int a = state_vals[i][k];
                int b = state_vals[i][k + 1];
                int c = state_vals[j][k];
                int d = state_vals[j][k + 1];
                int diff = (a + d) - (b + c);
                if (diff != 1 && diff != -1) { ok = false; break; }
            }
            if (ok) {
                size_t pos = adj_pos[i]++;
                adj[i][pos] = j;
            }
        }
    }
    free(adj_pos);
}

/* DP over rows: count total arrays of height (n+1). */
seq_t a(idx_t n)
{
    init_precompute();            /* ensure adjacency tables built */

    /* Static arrays that hold DP counts for each row. */
    static seq_t dp_cur[S];
    static seq_t dp_nxt[S];

    /* Pointers that we will swap each step. */
    static seq_t *dp_cur_ptr = dp_cur;
    static seq_t *dp_nxt_ptr = dp_nxt;

    /* Each single row can start the array. */
    for (int i = 0; i < S; ++i) {
        dp_cur_ptr[i] = 1;
        dp_nxt_ptr[i] = 0;
    }

    /* Transition for each additional row.  n steps give a (n+1)-row array. */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero next row counts */
        for (int i = 0; i < S; ++i) dp_nxt_ptr[i] = 0;

        for (int i = 0; i < S; ++i) {
            seq_t val = dp_cur_ptr[i];
            if (!val) continue;
            size_t len = adj_len[i];
            int *neigh = adj[i];
            for (size_t t = 0; t < len; ++t) {
                int j = neigh[t];
                dp_nxt_ptr[j] += val;
            }
        }
        /* swap pointers for next iteration */
        seq_t *tmp = dp_cur_ptr;
        dp_cur_ptr = dp_nxt_ptr;
        dp_nxt_ptr = tmp;
    }

    /* Sum all ways to finish in any row. */
    seq_t total = 0;
    for (int i = 0; i < S; ++i) total += dp_cur_ptr[i];
    return total;
}
