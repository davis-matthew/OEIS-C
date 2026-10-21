/*
 * A234202  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of all six edge and diagonal
 *          differences equal to 15.
 * https://oeis.org/A234202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic programming counting (n+1)×3 arrays of digits 0..5
   with every 2×2 subblock having the sum of the absolute values
   of all six edge and diagonal differences equal to 15.
   The function computes OEIS sequence A234202. */

#include <stdbool.h>
#include <stdlib.h>

static int val0[216], val1[216], val2[216];
static int *adj[216];
static int adj_len[216];
static bool graph_ready = false;

/* Build adjacency lists separating the transition graph */
static void init_graph(void) {
    /* Pre‑compute the three digits of each state */
    for (int s = 0; s < 216; ++s) {
        val0[s] = s / 36;
        val1[s] = (s / 6) % 6;
        val2[s] = s % 6;
    }

    /* Count edges to know how much memory to allocate */
    for (int s = 0; s < 216; ++s) {
        int cnt = 0;
        for (int t = 0; t < 216; ++t) {
            /* Sum of abs differences for the left 2×2 block */
            int s0 = val0[s], s1 = val1[s], s2 = val2[s];
            int t0 = val0[t], t1 = val1[t], t2 = val2[t];
            int sum1 = (s0>=s1? s0-s1:s1-s0) + (t0>=t1? t0-t1:t1-t0)
                     + (s0>=t0? s0-t0:t0-s0) + (s1>=t1? s1-t1:t1-s1)
                     + (s0>=t1? s0-t1:t1-s0) + (s1>=t0? s1-t0:t0-s1);
            if (sum1 != 15) continue;
            /* Sum for the right 2×2 block */
            int sum2 = (s1>=s2? s1-s2:s2-s1) + (t1>=t2? t1-t2:t2-t1)
                     + (s1>=t1? s1-t1:t1-s1) + (s2>=t2? s2-t2:t2-s2)
                     + (s1>=t2? s1-t2:t2-s1) + (s2>=t1? s2-t1:t1-s2);
            if (sum2 == 15) ++cnt;
        }
        adj_len[s] = cnt;
        if (cnt > 0)
            adj[s] = (int *)malloc(cnt * sizeof(int));
        else
            adj[s] = NULL;
    }

    /* Fill adjacency lists */
    int pos[216];
    for (int i = 0; i < 216; ++i) pos[i] = 0;
    for (int s = 0; s < 216; ++s) {
        for (int t = 0; t < 216; ++t) {
            int s0 = val0[s], s1 = val1[s], s2 = val2[s];
            int t0 = val0[t], t1 = val1[t], t2 = val2[t];
            int sum1 = (s0>=s1? s0-s1:s1-s0) + (t0>=t1? t0-t1:t1-t0)
                     + (s0>=t0? s0-t0:t0-s0) + (s1>=t1? s1-t1:t1-s1)
                     + (s0>=t1? s0-t1:t1-s0) + (s1>=t0? s1-t0:t0-s1);
            if (sum1 != 15) continue;
            int sum2 = (s1>=s2? s1-s2:s2-s1) + (t1>=t2? t1-t2:t2-t1)
                     + (s1>=t1? s1-t1:t1-s1) + (s2>=t2? s2-t2:t2-s2)
                     + (s1>=t2? s1-t2:t2-s1) + (s2>=t1? s2-t1:t1-s2);
            if (sum2 == 15) {
                adj[s][pos[s]++] = t;
            }
        }
    }

    graph_ready = true;
}

/* Vectors reused across calls */
static seq_t cur_vec[216];
static seq_t next_vec[216];

/* Memoisation of results */
static seq_t *dp = NULL;
static size_t dp_cap = 0;        /* allocated size for dp */
static size_t cur_len = 0;      /* last computed transition count */

/* Ensure dp array can hold at least n */
static void ensure_dp(size_t n) {
    if (dp_cap <= n) {
        size_t new_cap = dp_cap ? dp_cap * 2 : 4;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        dp = tmp;
        dp_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    if (!graph_ready) init_graph();

    if (dp == NULL) {
        cur_vec[0] = (seq_t)1;
        for (int i = 1; i < 216; ++i) cur_vec[i] = (seq_t)1;
        cur_len = 0;
        dp = (seq_t *)malloc(sizeof(seq_t));
        dp_cap = 1;
        dp[0] = (seq_t)0;
    }

    while (cur_len < n) {
        /* zero next_vec */
        for (int i = 0; i < 216; ++i) next_vec[i] = (seq_t)0;

        /* propagate transitions */
        for (int s = 0; s < 216; ++s) {
            if (adj_len[s] == 0) continue;
            seq_t v = cur_vec[s];
            if (v == (seq_t)0) continue;
            int *list = adj[s];
            int len = adj_len[s];
            for (int j = 0; j < len; ++j) {
                next_vec[list[j]] += v;
            }
        }

        /* swap vectors */
        for (int i = 0; i < 216; ++i) cur_vec[i] = next_vec[i];

        /* compute total for this length */
        seq_t total = (seq_t)0;
        for (int i = 0; i < 216; ++i) total += cur_vec[i];

        ++cur_len;
        ensure_dp(cur_len);
        dp[cur_len] = total;
    }

    return dp[n];
}
