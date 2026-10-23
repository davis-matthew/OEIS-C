/*
 * A015146  Triangle of (Gaussian) q-binomial coefficients for q=-21.
 * https://oeis.org/A015146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 22, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = value at index n in the triangle of q-binomial coefficients
 * for q = -21 read in row-major order.
 * The function builds the triangle row by row until the required index
 * is reached, then returns the requested entry.
 * It handles arbitrary index order and works with any integer type
 * defined for seq_t and idx_t.
 */
#include <stddef.h>
#ifdef __cplusplus
#include <new>
#endif

/* Helper functions for memory allocation of seq_t arrays. */
static inline seq_t* my_alloc_row(size_t n) {
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}
static inline void my_free_row(seq_t* p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    /* The triangle starts with 1. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Determine which row 'r' contains the desired index and the column 'k'. */
    idx_t r = 0;              /* current row number (starting from 0) */
    idx_t idx_start = 0;      /* starting index for the current row in the flat list */
    while (1) {
        idx_t next_idx = idx_start + r + 1;     /* first index after this row */
        if (n < next_idx)
            break;                             /* found the correct row */
        r++;
        idx_start = next_idx;
    }
    idx_t k = n - idx_start;   /* column within that row */

    /* q-value for the q-binomial (q = -21). */
    const seq_t Q = (seq_t)-21;

    /* Allocate and initialise the first row: C(0,0)=1. */
    seq_t *prev = my_alloc_row(1);
    prev[0] = (seq_t)1;

    /* factor holds q^(row-1) for the current row, initialised to q^0 = 1. */
    seq_t factor = (seq_t)1;

    /* Build rows incrementally until we reach row r. */
    for (idx_t row = 1; row <= r; ++row) {
        seq_t *curr = my_alloc_row((size_t)row + 1);
        curr[0] = (seq_t)1;                     /* C(row,0)=1 */
        curr[row] = (seq_t)1;                    /* C(row,row)=1 */

        /* power starts as q^(row-1). */
        seq_t power = factor;
        for (idx_t col = 1; col < row; ++col) {
            /* C(row,col) = C(row-1,col) + q^(row-col) * C(row-1,col-1) */
            curr[col] = prev[col] + power * prev[col - 1];
            /* Prepare q^(row-col-1) for the next column. */
            power /= (seq_t)Q;
        }

        /* Clean up the previous row and progress. */
        my_free_row(prev);
        prev = curr;

        /* Update factor for the next row: factor *= q gives q^row. */
        factor *= Q;
    }

    /* Retrieve the desired coefficient from the computed row. */
    seq_t result = prev[k];
    my_free_row(prev);
    return result;
}
