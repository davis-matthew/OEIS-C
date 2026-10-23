/*
 * A184788  Number of (n+2)X4 0..2 arrays with each 3X3 subblock singular.
 * https://oeis.org/A184788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of (n+2)×4 0..2 arrays with each 3×3 subblock singular
 *            computed by a DP over pairs of consecutive rows
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Number of possible 4‑digit rows in base‑3 */
#define ROWS 81
/* Number of pair states: (prev_row, curr_row) */
#define STATES (ROWS * ROWS)

/* 4‑digit representation of each row (base‑3 digits) */
static unsigned char row_digits[ROWS][4];

/* Edge: transition from state src to state dst */
typedef struct { uint16_t src; uint16_t dst; } Edge;

/* Transition table (built once) */
static Edge *edges = NULL;
static size_t edge_count = 0;

/* Two buffers for dynamic programming */
static seq_t dp_storage0[STATES];
static seq_t dp_storage1[STATES];

/* Flag to ensure one‑time initialization */
static bool initialized = false;

/* ----------------------------------------------------------------------
   Compute det of 3×3 matrix formed by rows a,b,c and columns x,y,z
   (0 ≤ x,y,z ≤ 3).  All entries are 0..2, so the result fits in int. */
static inline int det3(int a, int b, int c,
                      int x, int y, int z)
{
    int v1 = row_digits[a][x];
    int v2 = row_digits[a][y];
    int v3 = row_digits[a][z];
    int u1 = row_digits[b][x];
    int u2 = row_digits[b][y];
    int u3 = row_digits[b][z];
    int w1 = row_digits[c][x];
    int w2 = row_digits[c][y];
    int w3 = row_digits[c][z];

    return
        v1 * (u2 * w3 - u3 * w2) -
        v2 * (u1 * w3 - u3 * w1) +
        v3 * (u1 * w2 - u2 * w1);
}

/* ----------------------------------------------------------------------
   Build lookup tables once (rows, transitions). */
static void build_initialization(void)
{
    /* Generate all rows as base‑3 4‑digit vectors */
    for (int idx = 0; idx < ROWS; ++idx) {
        int val = idx;
        for (int d = 3; d >= 0; --d) {
            row_digits[idx][d] = (unsigned char)(val % 3);
            val /= 3;
        }
    }

    /* First pass: count valid triples to allocate edge list */
    size_t cnt = 0;
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            for (int c = 0; c < ROWS; ++c)
                if (det3(a, b, c, 0, 1, 2) == 0 &&
                    det3(a, b, c, 1, 2, 3) == 0)
                    ++cnt;

    edges = (Edge *)malloc(cnt * sizeof(Edge));
    edge_count = 0;
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b) {
            unsigned short src = (unsigned short)(a * ROWS + b);
            for (int c = 0; c < ROWS; ++c)
                if (det3(a, b, c, 0, 1, 2) == 0 &&
                    det3(a, b, c, 1, 2, 3) == 0) {
                    edges[edge_count].src = src;
                    edges[edge_count].dst = (unsigned short)(b * ROWS + c);
                    ++edge_count;
                }
        }

    initialized = true;
}

/* ----------------------------------------------------------------------
   Main sequence function.  Reinitialises DP buffers on every call. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* offset starts at 1 */

    if (!initialized)
        build_initialization();

    /* Re‑initialise dp_cur to ones and dp_next to zero for each call */
    seq_t *dp_cur  = dp_storage0;
    seq_t *dp_next = dp_storage1;

    for (size_t i = 0; i < STATES; ++i) {
        dp_cur[i]  = (seq_t)1;
        dp_next[i] = (seq_t)0;
    }

    /* DP over the rows */
    for (idx_t step = 1; step <= n; ++step) {
        /* zero dp_next for this iteration (already zero from init,
           but reused for safety) */
        for (size_t i = 0; i < STATES; ++i)
            dp_next[i] = (seq_t)0;

        /* propagate all allowed transitions */
        for (size_t e = 0; e < edge_count; ++e) {
            unsigned short src = edges[e].src;
            unsigned short dst = edges[e].dst;
            dp_next[dst] += dp_cur[src];
        }

        if (step == n) {
            /* sum over all terminal states */
            seq_t result = (seq_t)0;
            for (size_t i = 0; i < STATES; ++i)
                result += dp_next[i];
            return result;
        }

        /* swap dp buffers for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur  = dp_next;
        dp_next = tmp;
    }

    return (seq_t)0; /* should never reach */
}
