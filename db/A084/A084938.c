/*
 * A084938  Triangle read by rows: T(n,k) = Sum_{j>=0} j!*T(n-j-1, k-1) for n
 *          >= 0, k >= 0.
 * https://oeis.org/A084938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 105, u64 n <= 253,
 *            u128 n <= 666, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates OEIS A084938 by computing the triangle
   T(n,k) defined by
      T(n,k) = Sum_{j=0}^{n-k} j! * T(n-j-1, k-1)
   for n>=0, k>=0, with the base values
      T(0,0) = 1
      T(n,n) = 1   (0 <= n)
      T(n,0) = 0   for n>0
   The sequence a(n) is the triangle flattened by rows.
   (row 0: T(0,0); row 1: T(1,0),T(1,1); row 2: T(2,0),T(2,1),T(2,2); …)   */

#include <stddef.h>
#include <stdlib.h>

static seq_t **triangle = NULL;          /* array of pointers to each row */
static idx_t rows_computed = 0;          /* number of rows already allocated */

/* Ensure rows 0 .. needed_row inclusive are computed */
static void ensure_rows(idx_t needed_row)
{
    while (rows_computed <= needed_row)
    {
        idx_t n = rows_computed;
        /* allocate row n (length n+1) */
        seq_t *row = (seq_t*)calloc((n + 1), sizeof(seq_t));
        if (!row) return; /* allocation failure – abort quietly */

        /* base cases */
        if (n == 0)
        {
            row[0] = (seq_t)1;
        }
        else
        {
            row[0] = (seq_t)0;   /* T(n,0) = 0 for n>0 */
            row[n] = (seq_t)1;   /* T(n,n) = 1 */
        }

        /* compute interior entries: k = 1 .. n-1 */
        if (n >= 2)
        {
            for (idx_t k = 1; k < n; ++k)
            {
                seq_t sum = 0;
                seq_t fact = 1;          /* factorial of j */
                for (idx_t j = 0; j <= n - k; ++j)
                {
                    if (j > 0) fact *= (idx_t)j;
                    sum += fact * triangle[n - j - 1][k - 1];
                }
                row[k] = sum;
            }
        }

        /* store row pointer */
        triangle = (seq_t**)realloc(triangle, (rows_computed + 1) * sizeof(seq_t*));
        triangle[rows_computed] = row;
        ++rows_computed;
    }
}

seq_t a(idx_t n)
{
    /* find row and column that contains the n-th flattened element */
    idx_t row = 0;
    idx_t cum = 0;                     /* number of elements before current row */
    while (cum + (row + 1) <= n) {
        cum += (row + 1);
        ++row;
    }
    idx_t col = n - cum;

    /* compute rows up to 'row' if necessary */
    if (triangle == NULL) {
        triangle = NULL;
        rows_computed = 0;
    }
    ensure_rows(row);

    if (row >= rows_computed) return 0;   /* safety: should not happen */

    return triangle[row][col];
}
