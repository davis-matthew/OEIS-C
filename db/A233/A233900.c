/*
 * A233900  Number of (n+1)X(4+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the squares of all six edge and diagonal differences
 *          equal to 35.
 * https://oeis.org/A233900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A233900: number of (n+1)×5 arrays 0..6 where every 2×2 block has the sum
   of the squares of the six edge and diagonal differences equal to 35.
   The algorithm builds the graph whose vertices are the 7^5 possible 5‑tuples
   (rows) and where two rows are adjacent iff all four 2×2 blocks formed by
   them satisfy the required sum.  The number of arrays of length (n+1)
   equals the number of directed walks of length n in this graph.  The
   algorithm performs dynamic programming over the adjacency lists. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* The build supplies the following typedefs:
   typedef <unsigned integer of appropriate width> seq_t;
   typedef uint64_t idx_t; */

#define N_ROWS 16807ULL          /* 7^5 */

/* tables of all 16807 5‑digit base‑7 rows */
static uint8_t rows[N_ROWS][5];

/* 2×2 block validity table: 1 if sum of squared differences equals 35 */
static uint8_t block_valid[7][7][7][7];

/* adjacency lists for the compatibility graph (directed edges) */
static uint32_t *adj_start = NULL;   /* size N_ROWS+1, cumulative degrees */
static uint32_t *adj_data  = NULL;   /* concatenated neighbor lists */

/* DP buffers */
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;

/* one‑time initialization flag */
static bool initialized = false;

/* -------------------------------------------------------------------------- */
/* Precompute the block validity table. */
static void compute_block_table(void)
{
    for (int a = 0; a < 7; ++a)
        for (int b = 0; b < 7; ++b)
            for (int c = 0; c < 7; ++c)
                for (int d = 0; d < 7; ++d) {
                    int s = 0, diff;
                    diff = a - b; s += diff * diff;
                    diff = c - d; s += diff * diff;
                    diff = a - c; s += diff * diff;
                    diff = b - d; s += diff * diff;
                    diff = a - d; s += diff * diff;
                    diff = b - c; s += diff * diff;
                    block_valid[a][b][c][d] = (s == 35) ? 1 : 0;
                }
}

/* -------------------------------------------------------------------------- */
/* Generate all 7^5 rows as base‑7 5‑digit arrays. */
static void generate_rows(void)
{
    for (idx_t r = 0; r < N_ROWS; ++r) {
        idx_t t = r;
        for (int k = 4; k >= 0; --k) {
            rows[r][k] = t % 7;
            t /= 7;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Test whether two rows can be placed consecutively. */
static inline bool rows_compatible(idx_t i, idx_t j)
{
    return
        block_valid[rows[i][0]][rows[i][1]][rows[j][0]][rows[j][1]] &&
        block_valid[rows[i][1]][rows[i][2]][rows[j][1]][rows[j][2]] &&
        block_valid[rows[i][2]][rows[i][3]][rows[j][2]][rows[j][3]] &&
        block_valid[rows[i][3]][rows[i][4]][rows[j][3]][rows[j][4]];
}

/* -------------------------------------------------------------------------- */
/* Build adjacency lists (directed). */
static void build_adjacency(void)
{
    uint32_t *outdeg = (uint32_t *)calloc((size_t)N_ROWS, sizeof(uint32_t));
    uint32_t total_edges = 0;

    for (idx_t i = 0; i < N_ROWS; ++i) {
        for (idx_t j = i + 1; j < N_ROWS; ++j) {
            if (!rows_compatible(i, j))
                continue;
            outdeg[i]++; outdeg[j]++;
            total_edges += 2;            /* add both directions */
        }
    }

    adj_start = (uint32_t *)malloc(((size_t)N_ROWS + 1) * sizeof(uint32_t));
    adj_start[0] = 0;
    for (idx_t i = 0; i < N_ROWS; ++i)
        adj_start[i + 1] = adj_start[i] + outdeg[i];

    adj_data = (uint32_t *)malloc((size_t)total_edges * sizeof(uint32_t));

    /* temporary writers per row */
    uint32_t *cur = (uint32_t *)malloc((size_t)N_ROWS * sizeof(uint32_t));
    memcpy(cur, adj_start, (size_t)N_ROWS * sizeof(uint32_t));

    for (idx_t i = 0; i < N_ROWS; ++i) {
        for (idx_t j = i + 1; j < N_ROWS; ++j) {
            if (!rows_compatible(i, j))
                continue;
            adj_data[cur[i]++] = (uint32_t)j;
            adj_data[cur[j]++] = (uint32_t)i;
        }
    }

    free(outdeg);
    free(cur);
}

/* -------------------------------------------------------------------------- */
/* Allocate DP buffers. */
static void init_dp_buffers(void)
{
    dp_cur  = (seq_t *)malloc((size_t)N_ROWS * sizeof(seq_t));
    dp_next = (seq_t *)malloc((size_t)N_ROWS * sizeof(seq_t));
}

/* -------------------------------------------------------------------------- */
/* Single‑time initialization. */
static void init(void)
{
    if (initialized)
        return;
    compute_block_table();
    generate_rows();
    build_adjacency();
    init_dp_buffers();
    initialized = true;
}

/* -------------------------------------------------------------------------- */
/* Public sequence function. */
seq_t a(idx_t n)
{
    init();                       /* ensure all data structures exist */

    /* first row may be any of the 7^5 rows */
    for (size_t i = 0; i < (size_t)N_ROWS; ++i)
        dp_cur[i] = (seq_t)1;

    /* perform n transitions : produce an (n+1)×5 array */
    for (idx_t step = 0; step < n; ++step) {
        /* clear dp_next */
        memset(dp_next, 0, (size_t)N_ROWS * sizeof(seq_t));

        for (size_t i = 0; i < (size_t)N_ROWS; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0)
                continue;
            uint32_t start = adj_start[i];
            uint32_t end   = adj_start[i + 1];
            for (uint32_t k = start; k < end; ++k) {
                size_t j = (size_t)adj_data[k];
                dp_next[j] += val;
            }
        }
        /* swap current and next */
        seq_t *tmp = dp_cur;
        dp_cur    = dp_next;
        dp_next   = tmp;
    }

    /* sum over all ending rows */
    seq_t result = 0;
    for (size_t i = 0; i < (size_t)N_ROWS; ++i)
        result += dp_cur[i];

    return result;
}
