/*
 * A110165  Riordan array (1/sqrt(1-6x+5x^2),(1-3x-sqrt(1-6x+5x^2))/(2x)).
 * https://oeis.org/A110165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the concatenation of the rows of the triangle defined by
 *            T(n,k) = Σ_{j=0..n} C(n,j)∙C(2j , j+k), with 0 ≤ k ≤ n. Indices
 *            n are mapped to (row r, column k) by r = ⌊(√(1+8n)-1)/2⌋, k = n
 *            - r(r+1)/2. The function computes the requested entry using the
 *            recurrence T(0,0)=1, T(n,0)=3*T(n-1,0)+2...
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */
#include "../utils/tri.h"

/* ------------------------------------------------------------------ */
/* integer arithmetic helpers                                         */
/* ------------------------------------------------------------------ */

/* find the largest r such that r*(r+1)/2 ≤ n  */
static idx_t row_of_index(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    while (tri(high) <= n) {
        low = high;
        high <<= 1;           /* double until too large */
    }
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (tri(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* triangle storage and lazy generation                               */
/* ------------------------------------------------------------------ */

static seq_t **rows     = NULL;   /* rows[i] points to an array of length i+1 */
static size_t rows_cap  = 0;      /* allocated capacity of the rows array   */
static size_t rows_cnt  = 0;      /* number of rows already computed        */

/* ensure rows up to and including index r are computed and cached */
static void ensure_rows(idx_t r)
{
    while (rows_cnt <= (size_t)r)   /* compute next row */
    {
        size_t n     = rows_cnt;                                 /* current row index to compute                */
        seq_t *new_row = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_row) exit(EXIT_FAILURE);                       /* allocation failed                          */

        if (n == 0) {                                            /* base case */
            new_row[0] = (seq_t)1;
        } else {
            seq_t *prev = rows[n-1];                             /* previous row                               */

            /* k == 0 : T(n,0) = 3*T(n-1,0) + 2*T(n-1,1)        */
            new_row[0] = 3 * prev[0];
            if (n-1 >= 1) new_row[0] += 2 * prev[1];

            /* k >= 1 */
            for (size_t k = 1; k <= n; ++k) {
                seq_t val = prev[k-1];
                if (k <= n-1) val += 3 * prev[k];
                if (k+1 <= n-1) val += prev[k+1];
                new_row[k] = val;
            }
        }

        /* grow the rows array if necessary */
        if (rows_cnt >= rows_cap) {
            size_t new_cap = rows_cap ? 2 * rows_cap : 8;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            if (!tmp) exit(EXIT_FAILURE);  /* allocation failed  */
            rows          = tmp;
            rows_cap      = new_cap;
        }

        rows[rows_cnt] = new_row;
        rows_cnt++;
    }
}

/* ------------------------------------------------------------------ */
/* sequence entry function                                             */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);                             /* row index   */
    idx_t k = n - tri(r);                                  /* column index*/

    ensure_rows(r);                                         /* compute rows up to r */

    return rows[r][k];                                      /* requested entry */
}
