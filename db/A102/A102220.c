/*
 * A102220  Triangular matrix, read by rows, equal to [2*I - A008459]^(-1),
 *          i.e., the matrix inverse of the difference of twice the identity
 *          matrix and the triangular matrix of squared binomial coefficients.
 * https://oeis.org/A102220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(row,col)^2 * A(row-col), where A(0)=1 and for m>0:
 *            A(m)=sum_{k=1..m} C(m,k)^2 * A(m-k). Here (row,col) are obtained
 *            from n by the triangular indexing: n = row*(row+1)/2 + col, 0 <=
 *            col <= row
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 90, u128 n <= 230,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *T_col0 = NULL;      /* stores A(row) values */
static idx_t  T_cap   = 0;         /* allocated capacity */
static idx_t  T_size  = 0;         /* next unused index */

/* Ensure that A(0..row) are computed */
static void ensure_col0(idx_t row)
{
    if (row < T_size)
        return;

    /* Resize array if necessary */
    if (row + 1 > T_cap) {
        idx_t new_cap = T_cap ? T_cap : 1;
        while (new_cap <= row)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(T_col0, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* Out of memory */
        T_col0 = tmp;
        T_cap  = new_cap;
    }

    /* Compute new entries */
    for (idx_t m = T_size; m <= row; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;               /* C(m,0) */
        for (idx_t k = 1; k <= m; ++k) {
            binom = binom * (seq_t)(m - k + 1) / (seq_t)k;
            seq_t sq = binom * binom;
            sum += sq * T_col0[m - k];
        }
        T_col0[m] = (m == 0) ? 1 : sum;
    }
    T_size = row + 1;
}

/* Return the row index for position n (triangular indexing) */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Determine (row,col) for n */
    idx_t row = row_of(n);
    idx_t col = n - row * (row + 1) / 2;

    /* Make sure column‑0 values up to 'row' are available */
    ensure_col0(row);

    /* Compute binomial coefficient C(row,col) */
    seq_t binom = 1;
    /* Use the multiplicative formula */
    for (idx_t i = 1; i <= col; ++i) {
        binom = binom * (seq_t)(row - col + i) / (seq_t)i;
    }

    /* a(n) = C(row,col)^2 * A(row-col) */
    seq_t sq = binom * binom;
    return sq * T_col0[row - col];
}
