/*
 * A205834  Number of (n+1) X 7 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to two.
 * https://oeis.org/A205834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 arrays of 0..2 with exactly two
 *            clockwise edge * increases in every 2×2 subblock. The algorithm
 *            builds the 3^7=2187 possible * rows, precomputes adjacency
 *            between consecutive rows that satisfy the * 2‑increases
 *            condition, and then counts paths of length n+1 in the row gra...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Helpers:   valid_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/valid_pair.h"

#define N_ROWS 2187          /* 3^7 */
#define ROWS   N_ROWS

/* --------------------------------------------------------------------- */
/*  Row representation (base‑3 digits)                                  */
/* --------------------------------------------------------------------- */
static uint8_t rows[ROWS][7];

/* --------------------------------------------------------------------- */
/*  Adjacency lists: for each row i store start/end indices in a
 *  contiguous data array.                                            */
/* --------------------------------------------------------------------- */
static uint16_t *adj_data = NULL;      /* concatenated neighbour lists  */
static uint32_t *adj_start = NULL;     /* start index for each row      */
static bool      adj_built = false;

/* --------------------------------------------------------------------- */
/*  Helper: check whether two rows can be adjacent                     */

/* --------------------------------------------------------------------- */
/*  Build all 3^7 rows (base‑3 digits) and the adjacency lists           */
/* --------------------------------------------------------------------- */
static void build_rows_and_adj(void)
{
    if (adj_built) return;

    /* ---- Build rows ----------------------------------------------- */
    for (int idx = 0; idx < ROWS; ++idx)
    {
        int tmp = idx;
        for (int pos = 0; pos < 7; ++pos)
        {
            rows[idx][pos] = (uint8_t)(tmp % 3);
            tmp /= 3;
        }
    }

    /* ---- Compute each row's degree --------------------------------- */
    int *degree = (int*)malloc(ROWS * sizeof(int));
    int total_edges = 0;
    for (int i = 0; i < ROWS; ++i)
    {
        int cnt = 0;
        const uint8_t *ri = rows[i];
        for (int j = 0; j < ROWS; ++j)
            if (valid_pair(ri, rows[j])) ++cnt;
        degree[i] = cnt;
        total_edges += cnt;
    }

    /* ---- Allocate adjacency structure ----------------------------- */
    adj_start = (uint32_t*)calloc(ROWS + 1, sizeof(uint32_t));
    for (int i = 0; i < ROWS; ++i)
        adj_start[i+1] = adj_start[i] + degree[i];
    adj_data = (uint16_t*)malloc(total_edges * sizeof(uint16_t));

    /* ---- Fill adjacency data -------------------------------------- */
    int *pos = (int*)calloc(ROWS, sizeof(int));
    for (int i = 0; i < ROWS; ++i)
    {
        const uint8_t *ri = rows[i];
        int start = adj_start[i];
        int pp = 0;
        for (int j = 0; j < ROWS; ++j)
        {
            if (valid_pair(ri, rows[j]))
                adj_data[start + pp++] = (uint16_t)j;
        }
    }
    free(pos);

    free(degree);
    adj_built = true;
}

/* --------------------------------------------------------------------- */
/*  DP vectors (current and next state)                                */
/* --------------------------------------------------------------------- */
static seq_t *vec_cur = NULL;
static seq_t *vec_next = NULL;
static bool vec_built = false;

/* --------------------------------------------------------------------- */
/*  Initialise DP vectors once                                         */
/* --------------------------------------------------------------------- */
static void init_vec(void)
{
    if (vec_built) return;
    vec_cur  = (seq_t*)calloc(ROWS, sizeof(seq_t));
    vec_next = (seq_t*)calloc(ROWS, sizeof(seq_t));
    vec_built = true;
}

/* --------------------------------------------------------------------- */
/*  Main function                                                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n<1; return 0 */

    build_rows_and_adj();
    init_vec();

    /* start: one way to choose the first row */
    for (int i = 0; i < ROWS; ++i) vec_cur[i] = (seq_t)1;

    for (idx_t step = 1; step <= n; ++step)
    {
        /* zero next vector */
        memset(vec_next, 0, ROWS * sizeof(seq_t));

        /* propagate along edges */
        for (int i = 0; i < ROWS; ++i)
        {
            seq_t val = vec_cur[i];
            if (val == 0) continue;
            uint32_t start = adj_start[i];
            uint32_t end   = adj_start[i+1];
            for (uint32_t p = start; p < end; ++p)
            {
                uint16_t j = adj_data[p];
                vec_next[j] += val;
            }
        }

        /* swap current and next */
        seq_t *tmp = vec_cur;
        vec_cur   = vec_next;
        vec_next  = tmp;
    }

    /* sum all possible ending rows */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < ROWS; ++i) sum += vec_cur[i];

    return sum;
}
