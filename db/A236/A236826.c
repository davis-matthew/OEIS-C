/*
 * A236826  Number of (n+1) X (6+1) 0..2 arrays with the maximum plus the
 *          minimum minus the lower median of every 2 X 2 subblock equal.
 * https://oeis.org/A236826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 arrays over {0,1,2} such that the
 *            quantity max+min−lower‑median is equal for every 2×2 subblock.
 *            Computed by a graph‑based dynamic programming on the 2187
 *            possible rows
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   block_e, common_e
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/block_e.h"
#include "../utils/common_e.h"

#define ROWS 2187  /* 3^7 */

typedef struct { uint16_t j; uint8_t e; } Edge;

/* static data that is built once */
static bool built = false;
static unsigned char (*row_digits)[7] = NULL;  /* [ROWS][7] */
static size_t *offsets = NULL;                 /* [ROWS+1] */
static Edge *edges = NULL;                    /* adjacency list */
static size_t total_edges = 0;

static seq_t *dp_cur = NULL;      /* size ROWS*3 */
static seq_t *dp_next = NULL;     /* size ROWS*3 */
static seq_t *memo = NULL;        /* memo[n] = a(n) */
static idx_t memo_capacity = 0;
static idx_t computed = 0;        /* number of steps already computed */

static void build_graph(void)
{
    /* generate all rows */
    row_digits = (unsigned char (*)[7])malloc(ROWS * 7 * sizeof(uint8_t));
    for (int idx = 0; idx < ROWS; ++idx) {
        int val = idx;
        for (int pos = 6; pos >= 0; --pos) {
            row_digits[idx][pos] = val % 3;
            val /= 3;
        }
    }

    /* first pass: count outgoing edges per row */
    int *counts = (int *)malloc(ROWS * sizeof(int));
    memset(counts, 0, ROWS * sizeof(int));

    for (int i = 0; i < ROWS; ++i) {
        for (int j = i; j < ROWS; ++j) {
            int e = common_e(row_digits, i, j);
            if (e >= 0) {
                counts[i]++;
                if (i != j) counts[j]++;
            }
        }
    }

    /* offsets */
    offsets = (size_t *)malloc((ROWS + 1) * sizeof(size_t));
    offsets[0] = 0;
    for (int i = 0; i < ROWS; ++i)
        offsets[i + 1] = offsets[i] + (size_t)counts[i];
    total_edges = offsets[ROWS];

    /* allocate edge list */
    edges = (Edge *)malloc(total_edges * sizeof(Edge));

    /* second pass: fill lists */
    int *pos = (int *)malloc(ROWS * sizeof(int));
    for (int i = 0; i < ROWS; ++i) pos[i] = offsets[i];

    for (int i = 0; i < ROWS; ++i) {
        for (int j = i; j < ROWS; ++j) {
            int e = common_e(row_digits, i, j);
            if (e >= 0) {
                edges[pos[i]] = (Edge){ (uint16_t)j, (uint8_t)e };
                pos[i]++;
                if (i != j) {
                    edges[pos[j]] = (Edge){ (uint16_t)i, (uint8_t)e };
                    pos[j]++;
                }
            }
        }
    }

    free(counts);
    free(pos);
    built = true;
}

seq_t a(idx_t n)
{
    /* ensure data structures are ready */
    if (!built) build_graph();

    /* allocate dp arrays if first time */
    size_t dp_size = ROWS * 3;
    if (!dp_cur) {
        dp_cur = (seq_t *)calloc(dp_size, sizeof(seq_t));
        dp_next = (seq_t *)calloc(dp_size, sizeof(seq_t));
    }

    /* ensure memo capacity */
    if (memo_capacity < (size_t)(n + 1)) {
        size_t new_cap = n + 1;
        seq_t *new_memo = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (memo) {
            memcpy(new_memo, memo, memo_capacity * sizeof(seq_t));
            free(memo);
        }
        memo = new_memo;
        memo_capacity = new_cap;
    }

    /* compute steps up to n if necessary */
    while (computed < n) {
        memset(dp_next, 0, dp_size * sizeof(seq_t));
        if (computed == 0) {
            /* step 1: start from any single row */
            for (int i = 0; i < ROWS; ++i) {
                seq_t base = (seq_t)1;
                for (size_t p = offsets[i]; p < offsets[i + 1]; ++p) {
                    Edge e = edges[p];
                    dp_next[e.j * 3 + e.e] += base;
                }
            }
        } else {
            /* subsequent steps */
            for (int i = 0; i < ROWS; ++i) {
                size_t base_idx = i * 3;
                for (int e_val = 0; e_val < 3; ++e_val) {
                    seq_t val = dp_cur[base_idx + e_val];
                    if (val == (seq_t)0) continue;
                    for (size_t p = offsets[i]; p < offsets[i + 1]; ++p) {
                        Edge edge = edges[p];
                        if (edge.e != e_val) continue;
                        dp_next[edge.j * 3 + e_val] += val;
                    }
                }
            }
        }

        /* sum all entries for a(n) */
        seq_t sum = (seq_t)0;
        for (size_t idx = 0; idx < dp_size; ++idx) sum += dp_next[idx];
        memo[computed + 1] = sum;

        /* swap dp buffers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        computed++;
    }

    return memo[n];
}
