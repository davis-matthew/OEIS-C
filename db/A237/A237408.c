/*
 * A237408  Number of (n+1) X (4+1) 0..2 arrays with the maximum plus the
 *          upper median plus the minimum of every 2 X 2 subblock differing
 *          from its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..2 arrays of size (n+1)×5 satisfying that for
 *            every 2×2 subblock, (max + upper‑median + min) differs by
 *            exactly ±1 from its horizontal and vertical neighbor. Computed
 *            by dynamic programming on the graph of 5‑digit rows (base‑3)
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS     243             /* 3^5 rows */
#define COLS     5
#define MAX_EDGES (ROWS * ROWS)  /* upper bound on row‑to‑row edges */

/*  adjacency lists: edges from any row to any other row that satisfy
    horizontal constraints.  For each edge we store the four block values
    (max+upper‑median+min). */
static uint8_t  edge_from[MAX_EDGES];
static uint8_t  edge_to[MAX_EDGES];
static uint8_t  edge_val[MAX_EDGES][4];
static int      next_edge_idx[MAX_EDGES];   /* linked list per source row */

/*  For the vertical step, we need a list of successors for each edge.
    We store the successor list as a singly‑linked list of edge indices.
    Array sizes are generous enough for all possible transitions. */
#define MAX_ADJ (MAX_EDGES * 20 + 10) /* generous upper bound */
static int      first_succ[MAX_EDGES];
static int      succ_next[MAX_ADJ];
static int      succ_edge[MAX_ADJ];   /* edge index of successor */
static int      succ_cnt;

/*  Digits of all rows in base‑3 representation. */
static uint8_t row_digits[ROWS][COLS];
static int first_edge_from_row[ROWS];

/*  DP buffers reused across calls. */
static seq_t   *dp_buf = NULL;
static seq_t   *ndp_buf = NULL;
static size_t  dp_cap = 0;    /* capacity in number of edges */

/*  Build the row digit table once. */
static void
build_rows(void)
{
    uint16_t code;
    for (code = 0; code < ROWS; ++code) {
        uint16_t tmp = code;
        int k;
        for (k = COLS - 1; k >= 0; --k) {
            row_digits[code][k] = tmp % 3;
            tmp /= 3;
        }
    }
}

/*  Compute the block value: sum of min, upper‑median, and max of the four digits. */
static uint8_t
block_value(uint8_t d[4])
{
    /* Simple insertion sort for four elements. */
    int p, q;
    for (p = 1; p < 4; ++p)
        for (q = p; q > 0 && d[q-1] > d[q]; --q) {
            uint8_t t = d[q];
            d[q] = d[q-1];
            d[q-1] = t;
        }
    return d[0] + d[2] + d[3];
}

/*  Build the graph of edges and successor lists.  Runs once on first call. */
static void
build_graph(void)
{
    int ecnt = 0;

    for (int i = 0; i < ROWS; ++i)
        first_edge_from_row[i] = -1;

    for (int fr = 0; fr < ROWS; ++fr) {
        for (int to = 0; to < ROWS; ++to) {
            uint8_t val[4];
            int ok = 1;
            int j;
            for (j = 0; j < 4; ++j) {
                uint8_t d[4] = {
                    row_digits[fr][j],
                    row_digits[fr][j+1],
                    row_digits[to][j],
                    row_digits[to][j+1]
                };
                val[j] = block_value(d);
            }
            /* Horizontal ±1 constraint. */
            for (j = 0; j < 3; ++j) {
                int diff = (int)val[j] - (int)val[j+1];
                if (diff != 1 && diff != -1) {
                    ok = 0;
                    break;
                }
            }
            if (!ok) continue;

            edge_from[ecnt] = (uint8_t)fr;
            edge_to[ecnt]   = (uint8_t)to;
            memcpy(edge_val[ecnt], val, 4);
            next_edge_idx[ecnt] = first_edge_from_row[fr];
            first_edge_from_row[fr] = ecnt;
            ++ecnt;
        }
    }

    /* Build successor lists. */
    succ_cnt = 0;
    for (int e = 0; e < ecnt; ++e)
        first_succ[e] = -1;

    for (int e = 0; e < ecnt; ++e) {
        int first = first_edge_from_row[edge_to[e]];
        for (int f = first; f != -1; f = next_edge_idx[f]) {
            bool ok = true;
            int j;
            for (j = 0; j < 4; ++j) {
                int diff = (int)edge_val[e][j] - (int)edge_val[f][j];
                if (diff != 1 && diff != -1) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;

            /* Append successor. */
            succ_edge[succ_cnt] = f;
            succ_next[succ_cnt] = first_succ[e];
            first_succ[e] = succ_cnt;
            ++succ_cnt;
        }
    }

    /* Allocate DP buffers. */
    dp_cap = ecnt;
    dp_buf = (seq_t*)malloc(sizeof(seq_t) * dp_cap);
    ndp_buf = (seq_t*)malloc(sizeof(seq_t) * dp_cap);
}

/*  Main sequence function. */
seq_t
a(idx_t n)
{
    /* Build graph once. */
    static bool built = false;
    if (!built) {
        build_rows();
        build_graph();
        built = true;
    }

    int ecnt = (int)dp_cap;
    if (ecnt == 0) return 0;  /* safety */

    /* Base case: length 1 (two rows). */
    for (int i = 0; i < ecnt; ++i)
        dp_buf[i] = 1;

    if (n > 1) {
        /* DP over subsequent rows. */
        for (idx_t step = 2; step <= n; ++step) {
            /* Zero next buffer. */
            for (int i = 0; i < ecnt; ++i)
                ndp_buf[i] = 0;

            /* Transition. */
            int e;
            for (e = 0; e < ecnt; ++e) {
                seq_t cur = dp_buf[e];
                if (cur == 0) continue;
                int s = first_succ[e];
                while (s != -1) {
                    int fe = succ_edge[s];
                    ndp_buf[fe] += cur;
                    s = succ_next[s];
                }
            }

            /* Swap buffers. */
            seq_t *tmp = dp_buf;
            dp_buf = ndp_buf;
            ndp_buf = tmp;
        }
    }

    /* Sum all ways ending in any edge. */
    seq_t result = 0;
    for (int i = 0; i < ecnt; ++i)
        result += dp_buf[i];
    return result;
}
