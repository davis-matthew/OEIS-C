/*
 * A216220  Square array T, read by antidiagonals: T(n,k) = 0 if n-k>=2 or if
 *          k-n>=4, T(1,0) = T(0,0) = T(0,1) = T(0,2) = T(0,3) = 1, T(n,k) =
 *          T(n-1,k) + T(n,k-1).
 * https://oeis.org/A216220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,k) where (i,k) is obtained from n by antidiagonal *
 *            traversal of the square array T defined by: * T(n,k) = 0 if
 *            n-k>=2 or k-n>=4 * T(0,0)=T(0,1)=T(0,2)=T(0,3)=1 * T(1,0)=1 *
 *            T(n,k)=T(n-1,k)+T(n,k-1) otherwise * The sequence a(n) lists the
 *            entries of T in order of increasing * diagonal...
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for computed rows of T */
    static seq_t **T        = NULL;   /* array of row pointers   */
    static size_t *row_len  = NULL;   /* number of columns per row */
    static idx_t  *tri      = NULL;   /* cumulative counts N(s) */
    static idx_t  rows      = 0;       /* number of rows stored    */

    /* extend the table until it covers the required diagonal index s */
    while (rows == 0 || tri[rows-1] <= n) {
        idx_t idx = rows;                 /* row (and diagonal) index */
        size_t len = (size_t)idx + 4;      /* columns 0 .. idx+3   */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) return (seq_t)0;        /* allocation failure */

        /* initialise the row to zero */
        for (size_t p = 0; p < len; ++p) row[p] = (seq_t)0;

        /* base values */
        if (idx == 0) {
            for (size_t p = 0; p < 4; ++p) row[p] = (seq_t)1;
        } else if (idx == 1) {
            row[0] = (seq_t)1;
        }

        /* compute the rest of the row using the recurrence */
        for (size_t k = 0; k < len; ++k) {
            /* skip cells that are already set by the base conditions */
            if ((idx == 0 && k <= 3) || (idx == 1 && k == 0))
                continue;

            /* the recurrence applies only to the allowed k-range */
            if (k >= (idx >= 1 ? idx - 1 : 0) && k <= idx + 3) {
                seq_t up   = 0;
                seq_t left = 0;

                if (idx > 0 && k < row_len[idx-1]) /* T(n-1,k) */
                    up = T[idx-1][k];

                if (k > 0)                          /* T(n,k-1) */
                    left = row[k-1];

                row[k] = up + left;
            }
        }

        /* store the row, extending the auxiliary arrays */
        if (rows == 0) {
            T       = (seq_t **)malloc(sizeof(seq_t *));
            row_len = (size_t *)malloc(sizeof(size_t));
            tri     = (idx_t  *)malloc(sizeof(idx_t));
        } else {
            T       = (seq_t **)realloc(T,       (rows + 1) * sizeof(seq_t *));
            row_len = (size_t *)realloc(row_len, (rows + 1) * sizeof(size_t));
            tri     = (idx_t  *)realloc(tri,     (rows + 1) * sizeof(idx_t));
        }

        T[rows]       = row;
        row_len[rows] = len;
        tri[rows]     = ((idx + 1) * (idx + 2)) / 2;  /* N(s) */
        ++rows;
    }

    /* locate the cell (i,k) corresponding to index n */
    idx_t s = 0;
    while (s < rows && tri[s] <= n) ++s;
    idx_t prevTri = (s == 0) ? 0 : tri[s-1];
    idx_t j        = n - prevTri;      /* offset within the diagonal */
    idx_t i        = j;                /* row index                     */
    idx_t k        = s - j;            /* column index                  */

    /* if the requested column lies outside the computed range, the value is zero */
    if (k >= row_len[i])
        return (seq_t)0;
    return T[i][k];
}
