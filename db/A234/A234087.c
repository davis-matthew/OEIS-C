/*
 * A234087  Number of (n+1)X(4+1) 0..5 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 10.
 * https://oeis.org/A234087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A234087:
   Number of (n+1)×5 arrays with entries 0..5 such that for each 2×2 subblock the
   sum of the absolute values of the six edge‑and‑diagonal differences equals 10.
   Computation is based on dynamic programming over all 5‑tuples (6^5 = 7776 states). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif

/* Helper function: test compatibility of two 5‑tuples */
static bool compat(const uint8_t a[5], const uint8_t b[5])
{
    for (int i = 0; i < 4; ++i)
    {
        int d01 = (a[i] > a[i+1]) ? a[i]-a[i+1] : a[i+1]-a[i];
        int d02 = (a[i] > b[i])   ? a[i]-b[i]   : b[i]-a[i];
        int d13 = (a[i+1] > b[i+1]) ? a[i+1]-b[i+1] : b[i+1]-a[i+1];
        int d23 = (b[i] > b[i+1]) ? b[i]-b[i+1] : b[i+1]-b[i];
        int d03 = (a[i] > b[i+1]) ? a[i]-b[i+1] : b[i+1]-a[i];
        int d12 = (a[i+1] > b[i]) ? a[i+1]-b[i] : b[i]-a[i+1];
        int sum = d01 + d02 + d13 + d23 + d03 + d12;
        if (sum != 10) return false;
    }
    return true;
}

/* Precomputation data */
enum { NSTATE = 7776 };
static uint8_t patterns[NSTATE][5];
static uint16_t *adj_start = NULL;   /* size NSTATE+1 */
static uint16_t *adj_list  = NULL;   /* size = total edges */
static bool data_ready = false;

/* Initialize patterns and adjacency lists once */
static void build_data(void)
{
    if (data_ready) return;
    /* Generate all 5‑tuples in base‑6 */
    for (uint32_t code = 0; code < NSTATE; ++code)
    {
        uint32_t x = code;
        for (int k = 0; k < 5; ++k)
        {
            patterns[code][k] = (uint8_t)(x % 6);
            x /= 6;
        }
    }

    /* First pass: count outgoing edges per state */
    int *out_count = (int*)calloc(NSTATE, sizeof(int));
    for (int i = 0; i < NSTATE; ++i)
    {
        for (int j = 0; j < NSTATE; ++j)
        {
            if (compat(patterns[i], patterns[j]))
                ++out_count[i];
        }
    }

    /* Prefix sums for adjacency array indices */
    adj_start = (uint16_t*)malloc((NSTATE + 1) * sizeof(uint16_t));
    adj_start[0] = 0;
    for (int i = 0; i < NSTATE; ++i)
        adj_start[i+1] = adj_start[i] + (uint16_t)out_count[i];
    uint32_t total_edges = adj_start[NSTATE];

    adj_list = (uint16_t*)malloc(total_edges * sizeof(uint16_t));
    /* Second pass: fill adjacency lists */
    int *pos_in_row = (int*)calloc(NSTATE, sizeof(int));
    for (int i = 0; i < NSTATE; ++i)
    {
        uint16_t base = adj_start[i];
        for (int j = 0; j < NSTATE; ++j)
        {
            if (compat(patterns[i], patterns[j]))
            {
                uint16_t idx = base + (uint16_t)pos_in_row[i];
                adj_list[idx] = (uint16_t)j;
                ++pos_in_row[i];
            }
        }
    }
    free(out_count);
    free(pos_in_row);
    data_ready = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    build_data();   /* Ensure tables ready */
    /* DP arrays */
    seq_t *curr = (seq_t*)calloc(NSTATE, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(NSTATE, sizeof(seq_t));
    /* initialise: any first row is allowed */
    for (int i = 0; i < NSTATE; ++i)
        curr[i] = 1;

    for (idx_t step = 0; step < n; ++step)
    {
        /* zero next */
        memset(next, 0, NSTATE * sizeof(seq_t));
        /* propagate */
        for (int i = 0; i < NSTATE; ++i)
        {
            seq_t cnt = curr[i];
            if (!cnt) continue;
            uint16_t beg = adj_start[i];
            uint16_t end = adj_start[i+1];
            for (uint16_t e = beg; e < end; ++e)
            {
                uint16_t j = adj_list[e];
                next[j] += cnt;
            }
        }
        /* swap */
        seq_t *tmp = curr; curr = next; next = tmp;
    }

    /* Sum all ending rows */
    seq_t total = 0;
    for (int i = 0; i < NSTATE; ++i)
        total += curr[i];

    free(curr);
    free(next);
    return total;
}
