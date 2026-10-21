/*
 * A225201  Triangle (read by rows) of coefficients of the polynomials (in
 *          ascending order) of the denominators of the generalized sequence
 *          s(n) of the sum resp. product of generalized fractions f(n)
 *          defined recursively by f(1) = m/1; f(n+1) is chosen so that the
 *          sum and the product of the first n terms of t...
 * https://oeis.org/A225201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 19, i16 n <= 38, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle coefficients of denominators for the generalized sum/product
    sequence.  The coefficients are generated recursively:
        D_1(x) = 1
        D_{k+1}(x) = x^{2^{k-1}} * D_k(x) − D_k(x)^2
    The array rows[r] stores the coefficients of D_{r+1} in ascending
    order (constant term first).  The function returns a(idx_t n) as
    requested in the OEIS order.  */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t **rows = NULL;          /* array of coefficient arrays */
static size_t  row_count = 0;        /* number of rows currently stored */

static void compute_next_row(void)
{
    /* current last row has length prev_len = 1ULL << (row_count-1)   */
    size_t prev_len = 1ULL << (row_count - 1);   /* number of coeffs in current row */
    size_t next_len = prev_len << 1;            /* 2^(row_count) */

    seq_t *prev = rows[row_count - 1];
    seq_t *next = (seq_t *)malloc(sizeof(seq_t) * next_len);
    memset(next, 0, sizeof(seq_t) * next_len);

    /* shift by prev_len (multiplication by x^prev_len) */
    for (size_t i = 0; i < prev_len; ++i)
        next[i + prev_len] = prev[i];

    /* convolution prev * prev -> sq (size 2*prev_len-1) */
    size_t sq_len = 2 * prev_len - 1;
    seq_t *sq = (seq_t *)malloc(sizeof(seq_t) * sq_len);
    memset(sq, 0, sizeof(seq_t) * sq_len);

    for (size_t i = 0; i < prev_len; ++i) {
        const seq_t ai = prev[i];
        for (size_t j = 0; j < prev_len; ++j) {
            sq[i + j] += ai * prev[j];
        }
    }

    /* D_{k+1} = shift – sq  (subtract sq from the first sq_len terms) */
    for (size_t i = 0; i < sq_len; ++i)
        next[i] -= sq[i];

    free(sq);

    /* store the new row */
    seq_t **tmp = (seq_t **)realloc(rows, sizeof(seq_t *) * (row_count + 1));
    rows = tmp;
    rows[row_count] = next;
    ++row_count;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n=0 */

    /* determine the row that contains index n */
    size_t row = 1;
    while (((size_t)1 << row) - 1 < (size_t)n)
        ++row;                    /* pow2(row)-1 >= n  */

    /* compute rows up to the needed one */
    while (row_count < row) {
        if (row_count == 0) {
            /* initialise row 1: length 1, coefficient 1 */
            seq_t *first = (seq_t *)malloc(sizeof(seq_t));
            first[0] = 1;
            rows = (seq_t **)realloc(rows, sizeof(seq_t *));
            rows[0] = first;
            row_count = 1;
        } else {
            compute_next_row();
        }
    }

    size_t prev_total = ((size_t)1 << (row - 1)) - 1;   /* total terms before this row */
    size_t index_in_row = n - prev_total - 1;           /* 0‑based offset */

    return rows[row - 1][index_in_row];
}
