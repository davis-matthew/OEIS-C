/*
 * A237066  Number of (n+1)X(7+1) 0..2 arrays with the maximum plus the upper
 *          median plus the minimum minus the lower median of every 2X2
 *          subblock equal.
 * https://oeis.org/A237066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×8 arrays of digits 0..2 such that for
 *            every 2×2 subblock the value max + upper median + min – lower
 *            median is the same
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 9, u128 n <= 21,
 *            big any n
 * Helpers:   compute_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_f.h"

typedef struct {
    uint16_t *data;
    size_t size;
    size_t cap;
} List;

/* Number of possible rows: 3^8 = 6561. */
#define ROWS 6561

/* Precomputed data for all rows: the 8 digits of each row. */
static uint8_t rowDigits[ROWS][8];

/* Precomputation table for f(a,b,c,d) where a,b,c,d∈{0,1,2}. */
static uint8_t fTable[81];

/* Adjacency lists for each possible f‑value [0..4]. */
static List adjacency[5][ROWS];

/* Flag indicating whether precomputation has been performed. */
static int built = 0;

/* Grow a list to at least 'newcap' entries.  Works in C++ and C. */
static void grow_list(List *lst)
{
    size_t newcap = lst->cap ? (lst->cap << 1) : 16;
    uint16_t *tmp = (uint16_t *)realloc(lst->data, newcap * sizeof(uint16_t));
    if (!tmp) abort();                       /* memory exhausted */
    lst->data = tmp;
    lst->cap  = newcap;
}

/* Append an edge to an adjacency list. */
static void add_edge(int v, int from, int to)
{
    List *lst = &adjacency[v][from];
    if (lst->size == lst->cap) grow_list(lst);
    lst->data[lst->size++] = (uint16_t)to;
}

/* Perform all precomputation: row digits, f table, adjacency lists. */
static void build(void)
{
    /* Convert every integer 0..6560 into a base‑3 representation (8 digits). */
    for (int i = 0; i < ROWS; ++i) {
        int v = i;
        for (int j = 7; j >= 0; --j) {
            rowDigits[i][j] = (uint8_t)(v % 3);
            v /= 3;
        }
    }

    /* Build fTable for all 81 combinations of (a,b,c,d). */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d) {
                    int idx = ((a * 3 + b) * 3 + c) * 3 + d;
                    fTable[idx] = compute_f(a, b, c, d);
                }

    /* Initialise adjacency lists. */
    for (int k = 0; k < 5; ++k)
        for (int a = 0; a < ROWS; ++a) {
            adjacency[k][a].data = NULL;
            adjacency[k][a].size = 0;
            adjacency[k][a].cap  = 0;
        }

    /* For every ordered pair of rows, determine the common f‑value and
       store in the appropriate adjacency list. */
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            /* Compute f for the first column pair. */
            int v = fTable[((rowDigits[a][0] * 3 + rowDigits[a][1]) * 3
                            + rowDigits[b][0]) * 3 + rowDigits[b][1]];
            int ok = 1;
            for (int j = 1; j < 7; ++j) {
                int v2 = fTable[((rowDigits[a][j] * 3 + rowDigits[a][j + 1]) * 3
                                 + rowDigits[b][j]) * 3 + rowDigits[b][j + 1]];
                if (v2 != v) { ok = 0; break; }
            }
            if (ok) add_edge(v, a, b);
        }
    }

    built = 1;
}

/* Main sequence generator. */
seq_t a(idx_t n)
{
    if (!built) build();

    /* Two DP buffers: current and next row counts. */
    static seq_t dp[ROWS];
    static seq_t nxt[ROWS];
    seq_t *cur = dp;
    seq_t *nxt_arr = nxt;

    seq_t result = 0;

    /* For each possible f‑value (0..4) perform a DP over the rows. */
    for (int k = 0; k < 5; ++k) {
        /* Initially every row can be the first row. */
        for (int i = 0; i < ROWS; ++i) cur[i] = 1;

        /* We need n steps to create n+1 rows. */
        for (idx_t step = 0; step < n; ++step) {
            /* Zero the next buffer. */
            for (int i = 0; i < ROWS; ++i) nxt_arr[i] = 0;

            /* Transition through the adjacency list for this k. */
            for (int a = 0; a < ROWS; ++a) {
                seq_t val = cur[a];
                if (!val) continue;
                List *lst = &adjacency[k][a];
                for (size_t t = 0; t < lst->size; ++t) {
                    int b = lst->data[t];
                    nxt_arr[b] += val;
                }
            }

            /* Swap buffers for the next iteration. */
            seq_t *tmp = cur;
            cur = nxt_arr;
            nxt_arr = tmp;
        }

        /* Sum all counts in the final buffer. */
        seq_t sum = 0;
        for (int i = 0; i < ROWS; ++i) sum += cur[i];

        result += sum;
    }

    return result;
}
