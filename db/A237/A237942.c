/*
 * A237942  Number of (n+1)X(5+1) 0..2 arrays with the maximum minus the
 *          minimum of every 2X2 subblock equal.
 * https://oeis.org/A237942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Counting (n+1)×6 arrays with entries in {0,1,2} such that all 2×2
 * subblocks have the same max‑minus‑min value.  The value may be 0,1 or 2.
 * For each possible value r we build a directed graph whose vertices are
 * the 3^6 possible rows and whose edges connect pairs of rows that
 * produce r in every 2×2 window.  The number of arrays with a fixed
 * r is the number of walks of length n in that graph (n transitions
 * correspond to n+1 rows).  The total equals the sum over r=0,1,2.
 *
 * The code precomputes the adjacency lists for r=0,1,2 once and then
 * performs a DP for any requested n.  The implementation works with
 * any integer type for seq_t/idx_t that the build supplies.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define NUM_PATTERNS (729)     /* 3^6 */
#define WIDTH (6)
#define WINS 5                 /* number of 2×2 windows per pair of rows */

/* All 3^6 possible 6‑digit rows (digits 0..2). */
static int pattern[NUM_PATTERNS][WIDTH];

/* Adjacency lists: adj[r][i] holds indices of rows that can follow i
   while keeping max‑min = r for all 2×2 windows. */
static int *adj[3][NUM_PATTERNS];
static size_t adj_len[3][NUM_PATTERNS];
static size_t adj_cap[3][NUM_PATTERNS];

/* Flag indicating whether the tables are ready. */
static bool tables_built = false;

/* DP buffers for counting walks. */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static size_t dp_cap = 0;

/* Append a neighbor to an adjacency list, expanding the list if necessary. */
static void add_neighbor(int pattern_idx, int neighbor, int r)
{
    if (adj_len[r][pattern_idx] == adj_cap[r][pattern_idx]) {
        size_t new_cap = adj_cap[r][pattern_idx] * 2 + 1;
        int *tmp = (int *)realloc(adj[r][pattern_idx], new_cap * sizeof(int));
        if (!tmp) abort();
        adj[r][pattern_idx] = tmp;
        adj_cap[r][pattern_idx] = new_cap;
    }
    adj[r][pattern_idx][adj_len[r][pattern_idx]++] = neighbor;
}

/* Test if a single 2×2 block formed by rows p0 and p1 (columns k, k+1)
   has max‑min equal to r. */
static inline bool block_ok(const int *p0, const int *p1, int k, int r)
{
    int a = p0[k];
    int b = p0[k + 1];
    int c = p1[k];
    int d = p1[k + 1];

    int mn = a;
    int mx = a;
    if (b < mn) mn = b;
    if (c < mn) mn = c;
    if (d < mn) mn = d;
    if (b > mx) mx = b;
    if (c > mx) mx = c;
    if (d > mx) mx = d;

    return (mx - mn) == r;
}

/* Build pattern array and adjacency tables. */
static void build_tables(void)
{
    if (tables_built) return;

    /* Generate all 3^6 patterns. */
    for (int pat = 0; pat < NUM_PATTERNS; ++pat) {
        int v = pat;
        for (int pos = 0; pos < WIDTH; ++pos) {
            pattern[pat][pos] = v % 3;
            v /= 3;
        }
    }

    /* Allocate initial adjacency lists. */
    for (int r = 0; r <= 2; ++r) {
        for (int i = 0; i < NUM_PATTERNS; ++i) {
            adj_len[r][i] = 0;
            adj_cap[r][i] = 16;
            adj[r][i] = (int *)malloc(adj_cap[r][i] * sizeof(int));
            if (!adj[r][i]) abort();
        }
    }

    /* Populate adjacency lists. */
    for (int i = 0; i < NUM_PATTERNS; ++i) {
        const int *pi = pattern[i];
        for (int j = 0; j < NUM_PATTERNS; ++j) {
            const int *pj = pattern[j];
            for (int r = 0; r <= 2; ++r) {
                bool ok = true;
                for (int w = 0; w < WINS; ++w) {
                    if (!block_ok(pi, pj, w, r)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) add_neighbor(i, j, r);
            }
        }
    }

    tables_built = true;
}

/* Ensure DP buffers are allocated. */
static void ensure_dp_buffers(void)
{
    if (dp_cap >= NUM_PATTERNS) return;
    if (dp_cur) free(dp_cur);
    if (dp_next) free(dp_next);
    dp_cur = (seq_t *)malloc(NUM_PATTERNS * sizeof(seq_t));
    dp_next = (seq_t *)malloc(NUM_PATTERNS * sizeof(seq_t));
    if (!dp_cur || !dp_next) abort();
    dp_cap = NUM_PATTERNS;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but harmless */

    build_tables();
    ensure_dp_buffers();

    seq_t total = 0;

    for (int r = 0; r <= 2; ++r) {
        /* Start with one of each pattern for the first row. */
        for (int i = 0; i < NUM_PATTERNS; ++i) dp_cur[i] = 1;

        /* Perform n transitions (n edges) to reach n+1 rows. */
        for (idx_t step = 1; step <= n; ++step) {
            /* Zero the new array. */
            for (int i = 0; i < NUM_PATTERNS; ++i) dp_next[i] = 0;

            /* Propagate counts through the adjacency graph for this r. */
            for (int i = 0; i < NUM_PATTERNS; ++i) {
                seq_t ci = dp_cur[i];
                if (!ci) continue;
                size_t len = adj_len[r][i];
                int *neigh = adj[r][i];
                for (size_t k = 0; k < len; ++k) {
                    dp_next[neigh[k]] += ci;
                }
            }

            /* Swap buffers. */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }

        /* Sum counts for this r. */
        seq_t subtotal = 0;
        for (int i = 0; i < NUM_PATTERNS; ++i) subtotal += dp_cur[i];
        total += subtotal;
    }

    return total;
}
