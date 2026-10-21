/*
 * A060058  Triangle of numbers related to A000330 (sum of squares) and
 *          A000364 (Euler numbers).
 * https://oeis.org/A060058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 41, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   triangular_start
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A060058 – triangle of numbers related to A000330 (sum of squares)
    and A000364 (Euler numbers).
    Recurrence for the triangle a(n,m):

        a(0,0)   = 1
        a(n,-1)  = 0
        a(n,m)   = 0  if n < m
        a(n,m)   = a(n-1,m) + ((n+1-m)^2) * a(n,m-1),   n,m ≥ 0

    The sequence a(k) is obtained by listing the triangle rows
    consecutively (row 0, then row 1, …).  This function returns a(k)
    for any non‑negative index k; it builds the triangle lazily, memoising
    previously computed rows.  It uses only integer arithmetic and
    therefore works for any build where 'seq_t' is an unsigned integer
    type (including arbitrary‑precision types).  The routine is
    O(n) in the maximum row index needed, which is far below the
    2‑second limit for n ≈ 10000.  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/triangular_start.h"

static seq_t **triangle = NULL;   /* triangle[row][col] */
static idx_t   last_row = 0;       /* highest row computed (0‑based) */

/* find row r such that triangular_start(r) ≤ n < triangular_start(r+1) */
static inline idx_t row_from_index(idx_t n)
{
    idx_t r = 0;
    while (triangular_start(r + 1) <= n)
        ++r;
    return r;
}

/* ensure that triangle has rows up to and including 'row' */
static void ensure_row(idx_t row)
{
    if (triangle == NULL) {
        triangle = (seq_t**)malloc(sizeof(seq_t*));
        triangle[0] = (seq_t*)malloc(sizeof(seq_t));
        triangle[0][0] = (seq_t)1;
        last_row = 0;
    }
    if (row <= last_row)
        return;

    /* enlarge pointer array */
    triangle = (seq_t**)realloc(triangle, sizeof(seq_t*) * (row + 1));

    for (idx_t i = last_row + 1; i <= row; ++i) {
        triangle[i] = (seq_t*)malloc(sizeof(seq_t) * (i + 1));
        for (idx_t j = 0; j <= i; ++j) {
            seq_t prior = 0;     /* a(i-1, j) */
            seq_t left  = 0;     /* a(i, j-1) */

            if (j <= i - 1)
                prior = triangle[i - 1][j];
            if (j > 0)
                left = triangle[i][j - 1];

            if (j == 0) {
                /* a(i,0) = a(i-1,0) */
                triangle[i][j] = prior;
            } else {
                /* a(i,j) = a(i-1,j) + ((i+1-j)^2) * a(i,j-1) */
                idx_t factor = (i + 1 - j) * (i + 1 - j);
                seq_t prod = left * (seq_t)factor;
                triangle[i][j] = prior + prod;
            }
        }
    }
    last_row = row;
}

/* public sequence function: a(n) */
seq_t a(idx_t n)
{
    idx_t row = row_from_index(n);
    ensure_row(row);
    idx_t col = n - triangular_start(row);
    return triangle[row][col];
}
