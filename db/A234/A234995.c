/*
 * A234995  Number of (n+1) X (4+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A234995: Number of (n+1)×5 arrays of digits 0…6 such that
 *  (i) no two adjacent cells (horizontally or vertically) are equal,
 *  (ii) in every 2×2 subblock the two diagonal sums differ by exactly 2.
 *
 *  The algorithm enumerates all admissible rows (9072 in total) and
 *  builds a directed graph where an edge u→v exists iff two rows
 *  satisfy the vertical and 2×2 diagonal constraints.
 *  A linear DP over this graph yields the number of valid arrays of
 *  height n+1.  Results are memoised so that repeated calls are
 *  answered in O(1).
 *
 *  This implementation is pure integer arithmetic and is valid
 *  C11 and C++17.  All required integer types (seq_t, idx_t) are
 *  provided by the build system.  No external utilities are used.  */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Parameters and static tables                                       */
/* ------------------------------------------------------------------ */
#define ROWS  9072          /* 7*6*6*6*6 admissible rows */

/* row vectors, differences between adjacent columns,
 * adjacency list metadata                                            */
static uint8_t  row[ROWS][5];
static int8_t   diff[ROWS][4];
static uint8_t *deg = NULL;            /* degree of each row            */
static uint32_t *neigh_start = NULL;   /* start index in neigh_flat       */
static uint16_t *neigh_flat = NULL;    /* flattened adjacency lists      */

/* DP buffers and memoised results                                   */
static seq_t *dpA = NULL;
static seq_t *dpB = NULL;
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static seq_t *results = NULL;          /* results[i] = a(i), i≥0  */
static idx_t  res_size = 0;            /* allocated slots         */
static idx_t  cur_step = 0;            /* highest step computed   */

/* ------------------------------------------------------------------ */
/*  Helper: initialise rows, adjacency graph and DP buffers         */
/* ------------------------------------------------------------------ */
static void init(void)
{
    /* 1) generate all admissible rows */
    int rcnt = 0;
    for (int a = 0; a < 7; ++a)
    for (int b = 0; b < 7; ++b) if (b != a)
    for (int c = 0; c < 7; ++c) if (c != b)
    for (int d = 0; d < 7; ++d) if (d != c)
    for (int e = 0; e < 7; ++e) if (e != d)
    {
        row[rcnt][0] = (uint8_t)a;
        row[rcnt][1] = (uint8_t)b;
        row[rcnt][2] = (uint8_t)c;
        row[rcnt][3] = (uint8_t)d;
        row[rcnt][4] = (uint8_t)e;

        diff[rcnt][0] = (int8_t)(a - b);
        diff[rcnt][1] = (int8_t)(b - c);
        diff[rcnt][2] = (int8_t)(c - d);
        diff[rcnt][3] = (int8_t)(d - e);
        ++rcnt;
    }
    if (rcnt != ROWS) return;   /* should never happen */

    /* 2) compute degree counters */
    deg = (uint8_t *)malloc(ROWS * sizeof(uint8_t));
    if (!deg) return;
    memset(deg, 0, ROWS * sizeof(uint8_t));

    /* Compatibility predicate */
    for (int i = 0; i < ROWS; ++i) {
        for (int j = i + 1; j < ROWS; ++j) {
            /* vertical adjacency */
            if (row[i][0] == row[j][0]) continue;
            if (row[i][1] == row[j][1]) continue;
            if (row[i][2] == row[j][2]) continue;
            if (row[i][3] == row[j][3]) continue;
            if (row[i][4] == row[j][4]) continue;

            /* 2×2 diagonal‑sum condition */
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                int d = diff[i][k] - diff[j][k];
                if (d != 2 && d != -2) { ok = false; break; }
            }
            if (!ok) continue;

            ++deg[i];
            ++deg[j];
        }
    }

    /* 3) build adjacency lists, flattened */
    neigh_start = (uint32_t *)malloc(ROWS * sizeof(uint32_t));
    if (!neigh_start) return;
    uint32_t totalEdges = 0;
    neigh_start[0] = 0;
    for (int i = 1; i < ROWS; ++i) {
        totalEdges += deg[i-1];
        neigh_start[i] = totalEdges;
    }
    totalEdges += deg[ROWS-1];
    neigh_flat = (uint16_t *)malloc(totalEdges * sizeof(uint16_t));
    if (!neigh_flat) return;

    /* Temporary position array to fill neigh_flat */
    uint32_t *pos = (uint32_t *)malloc(ROWS * sizeof(uint32_t));
    if (!pos) return;
    memcpy(pos, neigh_start, ROWS * sizeof(uint32_t));

    for (int i = 0; i < ROWS; ++i) {
        for (int j = i + 1; j < ROWS; ++j) {
            /* vertical adjacency */
            if (row[i][0] == row[j][0]) continue;
            if (row[i][1] == row[j][1]) continue;
            if (row[i][2] == row[j][2]) continue;
            if (row[i][3] == row[j][3]) continue;
            if (row[i][4] == row[j][4]) continue;

            /* 2×2 diagonal‑sum condition */
            bool ok = true;
            for (int k = 0; k < 4; ++k) {
                int d = diff[i][k] - diff[j][k];
                if (d != 2 && d != -2) { ok = false; break; }
            }
            if (!ok) continue;

            neigh_flat[pos[i]++] = (uint16_t)j;
            neigh_flat[pos[j]++] = (uint16_t)i;
        }
    }
    free(pos);

    /* 4) initialise DP buffers */
    dpA = (seq_t *)malloc(ROWS * sizeof(seq_t));
    dpB = (seq_t *)malloc(ROWS * sizeof(seq_t));
    if (!dpA || !dpB) return;
    for (int i = 0; i < ROWS; ++i) dpA[i] = (seq_t)1;   /* one way for first row */
    dp_cur = dpA;
    dp_next = dpB;

    /* 5) memo table: results[0] corresponds to n=0 (a single row) */
    res_size = 1;
    results = (seq_t *)malloc(res_size * sizeof(seq_t));
    if (!results) return;
    results[0] = (seq_t)ROWS;   /* number of single‑row arrays */
    cur_step = 0;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Undefined for n < 1, but return 0 for safety */
    if (n == 0) return 0;

    /* Initialise once */
    if (!results) init();

    /* Ensure memo table can hold answers up to n */
    if (n >= res_size) {
        size_t new_size = res_size ? res_size << 1 : 1;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(results, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure: abort */
        memset(tmp + res_size, 0, (new_size - res_size) * sizeof(seq_t));
        results = tmp;
        res_size = new_size;
    }

    /* If already computed, return stored value */
    if (n <= cur_step) return results[n];

    /* Perform DP steps up to n */
    for (size_t step = cur_step + 1; step <= n; ++step) {
        /* zero next buffer */
        memset(dp_next, 0, ROWS * sizeof(seq_t));

        /* transitions */
        for (int i = 0; i < ROWS; ++i) {
            seq_t val = dp_cur[i];
            if (!val) continue;
            uint8_t d = deg[i];
            uint32_t base = neigh_start[i];
            for (uint8_t k = 0; k < d; ++k) {
                dp_next[ neigh_flat[ base + k ] ] += val;
            }
        }

        /* aggregate answer for this step */
        seq_t sum = 0;
        for (int i = 0; i < ROWS; ++i) sum += dp_next[i];
        results[step] = sum;

        /* swap buffers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    cur_step = n;
    return results[n];
}
