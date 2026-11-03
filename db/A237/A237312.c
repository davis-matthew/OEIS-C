/*
 * A237312  Number of (n+1)X(4+1) 0..3 arrays with the maximum plus the lower
 *          median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A237312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 204 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A237312 – Number of (n+1)×5 0..3 arrays with equal block value.
    The algorithm precomputes all 4‑digit rows (4⁵ = 1024) and for each
    possible constant c = max + median₂ – min of a 2×2 subblock
    (0…6) builds a directed graph of allowable transitions between rows.
    Then for a given n it counts all paths of length n on each graph
    and sums the results.  The graphs and row encodings are built once
    and reused across calls. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/*--------------------------------------------------------------------
 *  Computes the value of a 2×2 block.
 *  Input digits are 0–3 in already sorted order.
 *--------------------------------------------------------------------*/
static inline int block_value(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    uint8_t v[4] = { a, b, c, d };
    /* three comparison stages – the data is tiny, so a fast
       manual insertion sort is cheaper than any library routine */
    if (v[1] < v[0]) { uint8_t t = v[0]; v[0] = v[1]; v[1] = t; }
    if (v[2] < v[0]) { uint8_t t = v[0]; v[0] = v[2]; v[2] = t; }
    if (v[3] < v[0]) { uint8_t t = v[0]; v[0] = v[3]; v[3] = t; }
    if (v[2] < v[1]) { uint8_t t = v[1]; v[1] = v[2]; v[2] = t; }
    if (v[3] < v[1]) { uint8_t t = v[1]; v[1] = v[3]; v[3] = t; }
    if (v[3] < v[2]) { uint8_t t = v[2]; v[2] = v[3]; v[3] = t; }
    /* v[0] = min, v[1] = second smallest, v[3] = max */
    return (int)v[3] + (int)v[1] - (int)v[0];
}

/* Constants describing the board */
#define ROWS   1024          /* 4^5 rows (5 columns, digits 0…3) */
#define COLS   5
#define MAXC   6             /* maximum block value = max+median₂–min */
#define CNUM   (MAXC + 1)    /* number of possible constants */

/* Storage for a 4‑digit row in base‑4 */
static uint8_t row_val[ROWS][COLS];

/* Data structure for adjacency lists */
typedef struct {
    int *dest;            /* array of destination row indices */
    size_t len;           /* number of destinations */
} AdjList;

/* adjacency lists for each constant and each row */
static AdjList adj[CNUM][ROWS];

/* flag showing whether the static data has been built */
static bool initialized = false;

/*--------------------------------------------------------------------
 *  Build one‑time static data: all row encodings and adjacency lists.
 *--------------------------------------------------------------------*/
static void build_static(void)
{
    /* 1. enumerate all 4⁵ rows */
    for (int r = 0; r < ROWS; ++r) {
        int tmp = r;
        for (int c = COLS - 1; c >= 0; --c) {
            row_val[r][c] = (uint8_t)(tmp & 3); /* least significant digit */
            tmp >>= 2;
        }
    }

    /* 2. first pass – count edges for each (constant,row) pair */
    size_t edge_cnt[CNUM][ROWS] = { 0 };
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            /* value of the first subblock */
            int cval = block_value(row_val[a][0], row_val[a][1],
                                   row_val[b][0], row_val[b][1]);
            if (cval < 0 || cval > MAXC) continue; /* impossible, but safety */

            bool ok = true;
            for (int j = 1; j < COLS - 1 && ok; ++j) {
                int val = block_value(row_val[a][j], row_val[a][j + 1],
                                       row_val[b][j], row_val[b][j + 1]);
                if (val != cval) ok = false;
            }
            if (ok) edge_cnt[cval][a]++;
        }
    }

    /* 3. allocate adjacency lists with the exact needed sizes */
    for (int c = 0; c < CNUM; ++c) {
        for (int r = 0; r < ROWS; ++r) {
            size_t len = edge_cnt[c][r];
            if (len > 0) {
                adj[c][r].dest = (int *)malloc(len * sizeof(int));
                adj[c][r].len  = 0;
            } else {
                adj[c][r].dest = NULL;
                adj[c][r].len  = 0;
            }
        }
    }

    /* 4. second pass – actually store adjacency lists */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            int cval = block_value(row_val[a][0], row_val[a][1],
                                   row_val[b][0], row_val[b][1]);
            if (cval < 0 || cval > MAXC) continue;

            bool ok = true;
            for (int j = 1; j < COLS - 1 && ok; ++j) {
                int val = block_value(row_val[a][j], row_val[a][j + 1],
                                       row_val[b][j], row_val[b][j + 1]);
                if (val != cval) ok = false;
            }
            if (ok) {
                AdjList *al = &adj[cval][a];
                al->dest[al->len++] = b;
            }
        }
    }

    initialized = true;
}

/*--------------------------------------------------------------------
 *  Return the a(n) term of the sequence.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Build static data on first use */
    if (!initialized) build_static();

    seq_t total = 0;

    /* For each constant value we compute all (n+1)-row arrays */
    for (int c = 0; c < CNUM; ++c) {
        /* Quick check: if no row has outgoing edges for this c, skip */
        bool has_edge = false;
        for (int r = 0; r < ROWS; ++r) {
            if (adj[c][r].len > 0) { has_edge = true; break; }
        }
        if (!has_edge) continue;

        /* Allocate DP arrays */
        seq_t *cur  = mem_new(seq_t, ROWS);
        seq_t *next = mem_new(seq_t, ROWS);
        if (!cur || !next) {
            mem_free(cur); mem_free(next);
            return total;               /* unlikely, but safe */
        }

        /* Any starting row is allowed – initialize to 1 */
        seq_t one = 1;
        for (int r = 0; r < ROWS; ++r) cur[r] = one;

        /* Perform n transitions to get n+1 rows */
        for (idx_t step = 1; step <= n; ++step) {
            /* clear next */
            for (int r = 0; r < ROWS; ++r) next[r] = 0;

            /* propagate */
            for (int r = 0; r < ROWS; ++r) {
                if (cur[r] == 0) continue;
                AdjList *al = &adj[c][r];
                for (size_t k = 0; k < al->len; ++k) {
                    int b = al->dest[k];
                    next[b] += cur[r];
                }
            }

            /* swap */
            seq_t *tmp = cur; cur = next; next = tmp;
        }

        /* Sum all possible ending rows */
        for (int r = 0; r < ROWS; ++r) total += cur[r];

        mem_free(cur);
        mem_free(next);
    }

    return total;
}
