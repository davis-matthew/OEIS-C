/*
 * A233888  Number of (n+1)X(4+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 30, and no
 *          two adjacent values equal.
 * https://oeis.org/A233888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays over {0,…,6} with no two equal
 *            adjacent values and each 2×2 subblock having sum of squared edge
 *            differences = 30. Computed by dynamic programming over all
 *            allowed 5‑tuples
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   abs_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/abs_val.h"

/* Types supplied by the build. */
extern seq_t a(idx_t);

/* Parameters. */
#define MAX_N 10000                     /* maximum n to precompute */
#define NUM_ROWS 9072                   /* count of allowed 5‑tuples */

/* Returns the linear index of a 5‑tuple. */
static inline idx_t row_index(const uint8_t *a)
{
    return (a[0] * 46656u) + (a[1] * 7776u) +
           (a[2] * 1296u) + (a[3] * 216u) + (a[4] * 36u);
}

/* Checks that row i can precede row j. */
static bool good(idx_t i, idx_t j, const uint8_t (*rows)[5])
{
    /* no equal vertical neighbours */
    for (int col = 0; col < 5; ++col)
        if (rows[i][col] == rows[j][col]) return false;

    /* pre‑computed squares of 0..6 */
    static const int sq[7] = {0,1,4,9,16,25,36};
    for (int col = 0; col < 4; ++col) {
        int a = rows[i][col];
        int b = rows[i][col+1];
        int c = rows[j][col];
        int d = rows[j][col+1];
        int sum = sq[abs_val(a-b)] +
                  sq[abs_val(a-c)] +
                  sq[abs_val(b-d)] +
                  sq[abs_val(c-d)];
        if (sum != 30) return false;
    }
    return true;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    static bool init_done = false;
    static uint8_t (*rows)[5] = NULL;          /* all admissible rows          */
    static size_t *adj_start = NULL;          /* start index of adjacency list */
    static int   *adj = NULL;                 /* adjacency lists flattened     */
    static seq_t *prev_counts = NULL;         /* DP counts for current layer    */
    static seq_t *next_counts = NULL;         /* DP counts for next layer       */
    static seq_t *cache = NULL;               /* memoized a(k) values           */
    static idx_t cache_max = 0;               /* highest n whose value cached   */
    static size_t cache_cap = 0;              /* current cache capacity         */

    /* ---------- Lazy initialization ------------------------------------ */
    if (!init_done) {
        /* Generate all admissible rows. */
        rows = (uint8_t (*)[5])malloc(NUM_ROWS * sizeof(uint8_t[5]));
        idx_t r = 0;
        for (uint8_t a = 0; a < 7; ++a)
            for (uint8_t b = 0; b < 7; ++b) if (b != a)
                for (uint8_t c = 0; c < 7; ++c) if (c != b)
                    for (uint8_t d = 0; d < 7; ++d) if (d != c)
                        for (uint8_t e = 0; e < 7; ++e) if (e != d) {
                            rows[r][0] = a; rows[r][1] = b;
                            rows[r][2] = c; rows[r][3] = d;
                            rows[r][4] = e; ++r;
                        }

        /* Compute outdegrees. */
        size_t *deg = (size_t *)calloc(NUM_ROWS, sizeof(size_t));
        for (idx_t i = 0; i < NUM_ROWS; ++i)
            for (idx_t j = 0; j < NUM_ROWS; ++j)
                if (good(i, j, rows)) ++deg[i];

        /* Build adjacency structure. */
        adj_start = (size_t *)calloc(NUM_ROWS + 1, sizeof(size_t));
        adj_start[0] = 0;
        size_t total_edges = 0;
        for (idx_t i = 0; i < NUM_ROWS; ++i) {
            adj_start[i + 1] = adj_start[i] + deg[i];
            total_edges = adj_start[i + 1];
        }
        adj = (int *)malloc(total_edges * sizeof(int));
        size_t *cur = (size_t *)calloc(NUM_ROWS, sizeof(size_t));
        for (idx_t i = 0; i < NUM_ROWS; ++i)
            for (idx_t j = 0; j < NUM_ROWS; ++j)
                if (good(i, j, rows)) {
                    size_t pos = adj_start[i] + cur[i]++;
                    adj[pos] = (int)j;
                }
        free(deg); free(cur);

        /* DP arrays. */
        prev_counts = (seq_t *)calloc(NUM_ROWS, sizeof(seq_t));
        next_counts = (seq_t *)calloc(NUM_ROWS, sizeof(seq_t));

        /* Cache. */
        cache_cap = MAX_N + 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));

        init_done = true;
    }

    /* ---------- Extend cache up to n ----------------------------------- */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure */
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    for (idx_t k = cache_max + 1; k <= n; ++k) {
        if (k == 1) {
            /* One row can start an array. */
            for (idx_t i = 0; i < NUM_ROWS; ++i) prev_counts[i] = (seq_t)1;
        }

        /* Transition to next layer. */
        for (idx_t i = 0; i < NUM_ROWS; ++i) {
            seq_t val = prev_counts[i];
            if (val == (seq_t)0) continue;
            for (size_t e = adj_start[i]; e < adj_start[i + 1]; ++e) {
                int j = adj[e];
                next_counts[j] += val;
            }
        }

        /* Sum all rows to get a(k). */
        seq_t total = (seq_t)0;
        for (idx_t j = 0; j < NUM_ROWS; ++j) total += next_counts[j];
        cache[k] = total;

        /* Prepare for next iteration. */
        seq_t *tmp = prev_counts;
        prev_counts = next_counts;
        next_counts = tmp;
        for (idx_t i = 0; i < NUM_ROWS; ++i) next_counts[i] = (seq_t)0;
    }

    cache_max = n;
    return cache[n];
}
