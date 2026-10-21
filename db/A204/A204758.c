/*
 * A204758  Number of (n+1)X4 0..5 arrays with the permanents of all 2X2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays over {0,…,5} whose 2×2 subblock
 *            permanents are all equal and nonzero. (A204758)
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"

#define MAX_ROWS 1296          /* 6^4 possible rows */
#define CMAX   51              /* permanents 0..50, 0 unused */

struct Edge {
    uint16_t src;
    uint16_t dst;
    uint8_t  c;
};

static struct Edge *edges = NULL;
static size_t      edge_cnt = 0;
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;
static bool initialized = false;

static void build_edges(void)
{
    int digits[MAX_ROWS][4];
    for (int r = 0; r < MAX_ROWS; ++r) {
        int t = r;
        for (int k = 0; k < 4; ++k) {
            digits[r][k] = t % 6;
            t /= 6;
        }
    }

    size_t est = 0;
    for (int r1 = 0; r1 < MAX_ROWS; ++r1) {
        for (int r2 = 0; r2 < MAX_ROWS; ++r2) {
            int c1 = digits[r1][0] * digits[r2][1] + digits[r1][1] * digits[r2][0];
            int c2 = digits[r1][1] * digits[r2][2] + digits[r1][2] * digits[r2][1];
            int c3 = digits[r1][2] * digits[r2][3] + digits[r1][3] * digits[r2][2];
            if (c1 == c2 && c2 == c3 && c1 > 0)
                ++est;
        }
    }

    edges = (struct Edge*)malloc(est * sizeof(struct Edge));
    edge_cnt = 0;
    for (int r1 = 0; r1 < MAX_ROWS; ++r1) {
        for (int r2 = 0; r2 < MAX_ROWS; ++r2) {
            int c1 = digits[r1][0] * digits[r2][1] + digits[r1][1] * digits[r2][0];
            int c2 = digits[r1][1] * digits[r2][2] + digits[r1][2] * digits[r2][1];
            int c3 = digits[r1][2] * digits[r2][3] + digits[r1][3] * digits[r2][2];
            if (c1 == c2 && c2 == c3 && c1 > 0) {
                edges[edge_cnt].src = (uint16_t)r1;
                edges[edge_cnt].dst = (uint16_t)r2;
                edges[edge_cnt].c   = (uint8_t)c1;
                ++edge_cnt;
            }
        }
    }
}

/* Return a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        build_edges();
        dp_cur  = seq_alloc_new_array(MAX_ROWS * CMAX);
        dp_next = seq_alloc_new_array(MAX_ROWS * CMAX);
    }

    /* reset dp_cur to zero */
    for (size_t i = 0; i < (size_t)MAX_ROWS * CMAX; ++i)
        dp_cur[i] = (seq_t)0;

    /* first transition: choose second row, set permanent */
    for (size_t e = 0; e < edge_cnt; ++e) {
        const struct Edge *ed = &edges[e];
        dp_cur[(size_t)ed->dst * CMAX + ed->c] += (seq_t)1;
    }

    if (n == 1) {
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < (size_t)MAX_ROWS * CMAX; ++i)
            total += dp_cur[i];
        return total;
    }

    /* n-1 further transitions */
    for (size_t step = 1; step < (size_t)n; ++step) {
        for (size_t i = 0; i < (size_t)MAX_ROWS * CMAX; ++i)
            dp_next[i] = (seq_t)0;

        for (size_t e = 0; e < edge_cnt; ++e) {
            const struct Edge *ed = &edges[e];
            seq_t val = dp_cur[(size_t)ed->src * CMAX + ed->c];
            if (val != (seq_t)0)
                dp_next[(size_t)ed->dst * CMAX + ed->c] += val;
        }

        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    seq_t total = (seq_t)0;
    for (size_t i = 0; i < (size_t)MAX_ROWS * CMAX; ++i)
        total += dp_cur[i];
    return total;
}
