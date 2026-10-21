/*
 * A234668  Number of (n+1) X (2+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A234668: Number of (n+1)×3 arrays over {0,…,7} such that every
 * 2×2 subblock has its main‑diagonal sum differ from its
 * anti‑diagonal sum by ±4.
 *
 * The implementation uses a simple dynamic programming over the
 * 512 possible rows.  dp[i] holds the number of arrays of the
 * current length ending with row i.  For each step we form newdp
 * with the adjacency rules derived from the 2×2 constraints,
 * sum all newdp entries to obtain a(n), and then continue.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Constants for the state graph */
#define ROWS        512            /* 8³ possible rows (0–7 in base 8) */
#define MAX_ADJ     32             /* ≤32 outgoing edges per state */

/* Pre‑computed adjacency information */
static uint16_t adj[ROWS][MAX_ADJ];
static uint8_t  adj_cnt[ROWS];

/* DP arrays for the current and next step */
static seq_t dp[ROWS];
static seq_t newdp[ROWS];

/* Memoisation cache for a(1)…a(max_n) */
static seq_t *cached = NULL;   /* array of length cache_cap, 1‑based */
static size_t cache_cap = 0;   /* allocated size of cached */
static idx_t  cache_count = 0; /* number of values currently computed */

/* One‑time graph construction flag */
static bool graph_built = false;

/* ------------------------------------------------------------------ */
/* Build the graph of admissible row transitions                     */
/* ------------------------------------------------------------------ */
static void build_graph(void)
{
    uint8_t digits[ROWS][3];
    int i, j;
    int d0, d1;
    int cnt;

    /* decompose each row integer into its three base‑8 digits */
    for (i = 0; i < ROWS; ++i) {
        digits[i][0] = (uint8_t)(i     & 7);      /* least significant digit */
        digits[i][1] = (uint8_t)((i >> 3) & 7);
        digits[i][2] = (uint8_t)((i >> 6) & 7);    /* most significant digit */
    }

    /* for every pair of rows, test the two 2×2 constraints */
    for (i = 0; i < ROWS; ++i) {
        cnt = 0;
        for (j = 0; j < ROWS; ++j) {
            /* first 2×2 block: rows i (prev) and j (next) at cols 0‑1 */
            d0 = (int)digits[i][0] + (int)digits[j][1]
                 - (int)digits[i][1] - (int)digits[j][0];
            if (d0 != 4 && d0 != -4) continue;

            /* second 2×2 block: cols 1‑2 */
            d1 = (int)digits[i][1] + (int)digits[j][2]
                 - (int)digits[i][2] - (int)digits[j][1];
            if (d1 != 4 && d1 != -4) continue;

            adj[i][cnt++] = (uint16_t)j;
        }
        adj_cnt[i] = (uint8_t)cnt;
    }

    /* initialise dp for a single row: every row is possible once */
    for (i = 0; i < ROWS; ++i) dp[i] = 1;

    graph_built = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence routine                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* OEIS offset starts at n = 1 */
    if (n == 0) return 0;

    /* initialise graph and cache on first call */
    if (!graph_built) build_graph();
    if (cached == NULL) {
        cached = (seq_t*)calloc(1, sizeof(seq_t));  /* index 0 unused */
        cache_cap = 1;
        cache_count = 0;
    }

    /* extend the cache up to n, if necessary */
    while (cache_count < n) {
        idx_t step = cache_count + 1;   /* we need to compute a(step) */

        /* newdp array initially zero */
        memset(newdp, 0, sizeof(newdp));

        /* propagate one more row using adjacency */
        int i, k;
        for (i = 0; i < ROWS; ++i) {
            seq_t ways = dp[i];
            if (ways == 0) continue;
            for (k = 0; k < adj_cnt[i]; ++k) {
                uint16_t j = adj[i][k];
                newdp[j] += ways;
            }
        }

        /* sum all ways after this step => a(step) */
        seq_t total = 0;
        for (i = 0; i < ROWS; ++i) total += newdp[i];

        /* store the result in the cache */
        if ((size_t)step >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < (size_t)step + 1) new_cap = (size_t)step + 1;
            seq_t *tmp = (seq_t*)realloc(cached, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
            cached = tmp;
            /* initialise newly allocated memory to zero */
            memset(cached + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
            cache_cap = new_cap;
        }
        cached[step] = total;

        /* prepare for next iteration */
        memcpy(dp, newdp, sizeof(dp));
        cache_count = step;
    }

    return cached[n];
}
