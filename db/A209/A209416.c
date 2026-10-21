/*
 * A209416  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209415; see the Formula section.
 * https://oeis.org/A209416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the triangle T described in the OEIS
 *            entry. T is defined by T(1,1) = 1, T(2,1) = 1, T(2,2) = 2,
 *            T(r,1) = 1 for all r ≥ 1, for r ≥ 3 and 2 ≤ k ≤ r T(r,k) =
 *            2·T(r-1,k-1) + T(r-2,k) + T(r-2,k-1) – T(r-2,k-2) where
 *            out‑of‑range indices are treated as 0. The sequence a(n) is...
 * Range:     u8 n <= 40, u16 n <= 127, u32 n <= 419, u64 n <= 1225,
 *            u128 n <= 1225, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 bigint_t;   /* use 128‑bit arithmetic if available   */
#else
typedef seq_t bigint_t;               /* otherwise fall back to the seq_t type */
#endif

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; value for n==0 is undefined */
        return (seq_t)0;

    /* determine the row r such that 1 + 2 + … + r ≥ n */
    idx_t r = 1;
    while (((seq_t)r * (r + 1)) / 2 < n)
        ++r;

    /* column index (1‑based) within this row */
    size_t prev_sum = (r - 1) * r / 2;     /* integer arithmetic fits in size_t */
    size_t col = n - prev_sum;            /* 1 ≤ col ≤ r */

    /* static storage of already computed rows of the triangle */
    static seq_t **triangle = NULL;       /* triangle[i] is row (i+1) */
    static size_t rows_allocated = 0;     /* number of rows currently allocated */

    /* compute missing rows up to r */
    if ((size_t)r > rows_allocated) {
        size_t new_rows = r;
        triangle = (seq_t **)realloc(triangle, new_rows * sizeof(seq_t *));
        for (size_t i = rows_allocated; i < new_rows; ++i) {
            size_t row_len = i + 1;                     /* row number (1‑based)   */
            seq_t *row = (seq_t *)calloc(row_len + 1, sizeof(seq_t));
            if (!row) exit(EXIT_FAILURE);               /* simple error handling  */
            triangle[i] = row;

            if (row_len == 1) {                         /* T(1,1) = 1                 */
                row[1] = (seq_t)1;
            } else if (row_len == 2) {                  /* T(2,1)=1, T(2,2)=2          */
                row[1] = (seq_t)1;
                row[2] = (seq_t)2;
            } else {                                    /* compute for r ≥ 3            */
                row[1] = (seq_t)1;
                for (size_t k = 2; k <= row_len; ++k) {
                    bigint_t val = (bigint_t)2 * (bigint_t)triangle[i-1][k-1];
                    if (k <= row_len - 2)            val += (bigint_t)triangle[i-2][k];
                    if (k - 1 <= row_len - 2)        val += (bigint_t)triangle[i-2][k-1];
                    if (k >= 3)                      val -= (bigint_t)triangle[i-2][k-2];
                    row[k] = (seq_t)val;
                }
            }
        }
        rows_allocated = new_rows;
    }

    /* return the requested element */
    return triangle[r-1][col];
}
