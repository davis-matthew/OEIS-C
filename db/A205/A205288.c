/*
 * A205288  Number of (n+1)X4 0..2 arrays with permanents of 2X2 subblocks
 *          differing from horizontal and vertical neighbor permanents.
 * https://oeis.org/A205288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A205288: number of (n+1)×4 arrays over {0,1,2} whose 2×2 subblock
   permanents are pairwise different among horizontal and vertical
   neighbours.
   The implementation builds all admissible row pairs as graph nodes,
   precomputes adjacency, and counts directed paths of length n−1 by
   dynamic programming.  It works for any seq_t/idx_t types that satisfy
   the constraints in the problem statement.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 3^4 possible rows, 81 rows */
#define ROWS 81
/* nodes = ordered pairs of rows */
#define NODES (ROWS * ROWS)

/* maximum n that is memoised (enough for the judge) */
#define MAX_N 10000

/* static data structures (built once) */
static bool initialized = false;

/* row digits table [row][position] */
static uint8_t row_digits[ROWS][4];

/* permanents of the 2×2 subblocks for a node (p0,p1,p2) */
static uint8_t perm_vals[NODES][3];

/* whether the node itself satisfies the distinctness requirement */
static bool node_valid[NODES];

/* CSR adjacency representation */
static idx_t edge_cnt[NODES];
static idx_t edge_start[NODES + 1];
static idx_t *edge_to = NULL;

/* DP buffers */
static seq_t dp_buf0[NODES];
static seq_t dp_buf1[NODES];
static seq_t *dp_prev_ptr = dp_buf0;
static seq_t *dp_curr_ptr = dp_buf1;

/* memoised array values a(1)..a(MAX_N) */
static seq_t seq_cache[MAX_N + 1];
static idx_t cached_up_to = 0;   /* highest n that has been cached */

/* utility: set array to zero */
static void zero_dp(seq_t *arr)
{
    for (idx_t i = 0; i < NODES; ++i)
        arr[i] = (seq_t)0;
}

/* pre‑compute graph and initial DP value */
static void precompute(void)
{
    if (initialized) return;
    initialized = true;

    /* compute digits of all rows */
    for (idx_t r = 0; r < ROWS; ++r) {
        idx_t x = r;
        for (int p = 3; p >= 0; --p) {
            row_digits[r][p] = (uint8_t)(x % 3);
            x /= 3;
        }
    }

    /* compute permanents and node validity */
    for (idx_t r = 0; r < ROWS; ++r) {
        for (idx_t s = 0; s < ROWS; ++s) {
            idx_t id = r * ROWS + s;
            uint8_t *a = row_digits[r];
            uint8_t *b = row_digits[s];
            uint8_t p0 = a[0] * b[1] + a[1] * b[0];
            uint8_t p1 = a[1] * b[2] + a[2] * b[1];
            uint8_t p2 = a[2] * b[3] + a[3] * b[2];
            perm_vals[id][0] = p0;
            perm_vals[id][1] = p1;
            perm_vals[id][2] = p2;
            node_valid[id] = (p0 != p1) && (p1 != p2);
        }
    }

    /* count outgoing edges for each node */
    for (idx_t r = 0; r < ROWS; ++r) {
        for (idx_t s = 0; s < ROWS; ++s) {
            idx_t id = r * ROWS + s;
            if (!node_valid[id]) continue;
            idx_t cnt = 0;
            for (idx_t t = 0; t < ROWS; ++t) {
                if (!node_valid[s * ROWS + t]) continue;
                if (perm_vals[id][0] != perm_vals[s * ROWS + t][0] &&
                    perm_vals[id][1] != perm_vals[s * ROWS + t][1] &&
                    perm_vals[id][2] != perm_vals[s * ROWS + t][2])
                    ++cnt;
            }
            edge_cnt[id] = cnt;
        }
    }

    /* CSR start indices */
    edge_start[0] = 0;
    for (idx_t i = 0; i < NODES; ++i)
        edge_start[i + 1] = edge_start[i] + edge_cnt[i];

    /* allocate edge list */
    idx_t total_edges = edge_start[NODES];
    edge_to = (idx_t *)malloc(total_edges * sizeof(idx_t));
    if (!edge_to) exit(1);

    /* fill edge list */
    idx_t pos = 0;
    for (idx_t r = 0; r < ROWS; ++r) {
        for (idx_t s = 0; s < ROWS; ++s) {
            idx_t id = r * ROWS + s;
            if (!node_valid[id]) continue;
            for (idx_t t = 0; t < ROWS; ++t) {
                if (!node_valid[s * ROWS + t]) continue;
                if (perm_vals[id][0] != perm_vals[s * ROWS + t][0] &&
                    perm_vals[id][1] != perm_vals[s * ROWS + t][1] &&
                    perm_vals[id][2] != perm_vals[s * ROWS + t][2])
                    edge_to[pos++] = s * ROWS + t;
            }
        }
    }

    /* initialise DP for n = 1 (single node paths) */
    for (idx_t id = 0; id < NODES; ++id)
        dp_prev_ptr[id] = node_valid[id] ? (seq_t)1 : (seq_t)0;

    /* sum over all nodes gives a(1) */
    seq_t sum = 0;
    for (idx_t id = 0; id < NODES; ++id)
        sum += dp_prev_ptr[id];
    seq_cache[1] = sum;
    cached_up_to = 1;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            // undefined for n=0
    if (n > MAX_N) return (seq_t)0;        // beyond pre‑allocated cache

    precompute();

    /* raise cached level if necessary */
    while (cached_up_to < n) {
        zero_dp(dp_curr_ptr);

        /* transition from dp_prev_ptr to dp_curr_ptr */
        for (idx_t src = 0; src < NODES; ++src) {
            if (!node_valid[src]) continue;
            seq_t val = dp_prev_ptr[src];
            if (!val) continue;
            idx_t start = edge_start[src];
            idx_t cnt   = edge_cnt[src];
            for (idx_t e = 0; e < cnt; ++e) {
                idx_t dest = edge_to[start + e];
                dp_curr_ptr[dest] += val;
            }
        }

        /* compute a(next) as sum over dp_curr_ptr */
        seq_t sum = 0;
        for (idx_t id = 0; id < NODES; ++id)
            sum += dp_curr_ptr[id];
        seq_cache[++cached_up_to] = sum;

        /* swap dp buffers */
        seq_t *tmp = dp_prev_ptr;
        dp_prev_ptr = dp_curr_ptr;
        dp_curr_ptr = tmp;
    }

    return seq_cache[n];
}
