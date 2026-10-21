/*
 * A060074  Triangle A060058 by diagonals.
 * https://oeis.org/A060074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   Flattened triangle A060058 defined by a(n,m) = a(n-1,m-1) +
 *            (m+1)^2 * a(n,m+1), with a(0,0)=1, a(n,-1)=0 and a(n,m)=0 if
 *            n<m. a(k) is the row‑major flattened entry a(r,j) where r is the
 *            row containing the index, and j is the column index within that
 *            row. Offset 0
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   mul_small, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_alloc.h"   /* seq_alloc(size_t) */
#include "../utils/mul_small.h"   /* mul_small(seq_t, unsigned int) */


static seq_t **triangle_rows = NULL;     /* pointers to each computed row */
static idx_t  rows_cap     = 0;           /* number of rows currently stored */

/* Compute rows 0 … max_row inclusive and store in triangle_rows. */
static void compute_rows_upto(idx_t max_row)
{
    if (max_row < rows_cap)
        return;                         /* already available */

    /* Resize the array of row pointers. */
    seq_t **new_rows = (seq_t **)realloc(triangle_rows,
                                         (max_row + 1) * sizeof(seq_t *));
    triangle_rows = new_rows;

    for (idx_t r = rows_cap; r <= max_row; ++r)
    {
        triangle_rows[r] = seq_alloc((size_t)(r + 1));
        seq_t *row = triangle_rows[r];

        if (r == 0)                      /* base row */
        {
            row[0] = 1;
        }
        else
        {
            row[r] = 1;                   /* diagonal entry a(r,r)=1 */

            /* Fill the rest of the row from right to left. */
            for (int m = (int)r - 1; m >= 0; --m)
            {
                seq_t prev = (m == 0) ? 0 : triangle_rows[r - 1][m - 1];
                unsigned int mu = (unsigned int)(m + 1);      /* m+1 */
                seq_t term = mul_small(row[m + 1], mu * mu);
                row[m] = prev + term;
            }
        }
    }
    rows_cap = max_row + 1;
}

/* Return the row index that contains global index n. */
static idx_t row_from_index(idx_t n)
{
    idx_t i = 0;
    idx_t total = 1;                     /* elements up to and including row i */

    while (n >= total)
    {
        ++i;
        total += i + 1;                  /* next row has length i+1 */
    }
    return i;
}

/* External linkage function required by the tests. */
seq_t a(idx_t n)
{
    idx_t r = row_from_index(n);
    idx_t j = n - r * (r + 1) / 2;        /* column position in row r */

    compute_rows_upto(r);                /* ensure row r is available */
    return triangle_rows[r][j];
}
