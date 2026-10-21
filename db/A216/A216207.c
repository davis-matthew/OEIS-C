/*
 * A216207  Matrix inverse of A181543 (cubes of entries of Pascal's triangle).
 * https://oeis.org/A216207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 27, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A216207:  Matrix inverse of A181543 (cubes of entries of Pascal's triangle)
    a(n) is the row‑major enumeration of the lower‑triangular inverse matrix B,
    where B_{n,n} = 1 and for k < n:
        B_{n,k} = - Σ_{m=k}^{n-1} binom(n,m)^3 * B_{m,k}
    Index n is converted to row r and column k by:
        r = floor((√(8n+1) – 1) / 2)
        k = n – r(r+1)/2
*/

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build */
static seq_t **rows      = NULL; /* pointers to each row */
static idx_t  rows_count = 0;    /* number of rows currently computed */
static idx_t  rows_cap   = 0;    /* capacity of the rows pointer array */

/* Ensure that rows[0..rmax] are computed. */
static void ensure_rows(idx_t rmax)
{
    if (rows == NULL) /* allocate rows pointer array once */
    {
        rows_cap = 1;
        rows = (seq_t **)malloc(rows_cap * sizeof(seq_t *));
        if (!rows) exit(1);
    }

    /* Grow the rows pointer array if needed */
    while (rows_count <= rmax)
    {
        if (rows_count + 1 > rows_cap)
        {
            rows_cap = rows_cap ? rows_cap * 2 : 1;
            seq_t **tmp = (seq_t **)realloc(rows, rows_cap * sizeof(seq_t *));
            if (!tmp) exit(1);
            rows = tmp;
        }

        idx_t r = rows_count;                     /* new row index */
        seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (!row) exit(1);

        if (r == 0)
        {
            row[0] = 1;                      /* B[0,0] = 1 */
        }
        else
        {
            /* Pre‑compute binomial coefficients C(r,m) for m=0..r-1 */
            seq_t *bin = (seq_t *)malloc(r * sizeof(seq_t));
            if (!bin) exit(1);
            bin[0] = 1;
            for (idx_t m = 0; m + 1 < r; ++m)
            {
                bin[m + 1] = bin[m] * (seq_t)(r - m) / (m + 1);
            }

            /* Compute off‑diagonal entries */
            for (idx_t k = 0; k < r; ++k)
            {
                seq_t sum = 0;
                for (idx_t m = k; m < r; ++m)
                {
                    seq_t bin_val = bin[m];
                    seq_t cube = bin_val * bin_val * bin_val;
                    seq_t term = cube * rows[m][k];
                    sum += term;
                }
                row[k] = -sum;
            }

            row[r] = 1;                       /* diagonal entry */
            free(bin);
        }

        rows[rows_count++] = row;
    }
}

/* Find r such that r*(r+1)/2 <= n < (r+1)(r+2)/2 */
static idx_t row_of_index(idx_t n)
{
    idx_t low = 0, high = n, r = 0;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 start = (unsigned __int128)mid * (mid + 1) / 2;
        if (start <= n)
        {
            r = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t k = n - r * (r + 1) / 2;
    ensure_rows(r);
    return rows[r][k];
}
