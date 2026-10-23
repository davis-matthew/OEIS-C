/*
 * A101494  Triangle, read by rows, where T(n,k) = Sum_{j=0..n-k-1}
 *          C(j+k,j)*T(n-1,j+k) for n>k>=0 with T(n,n)=1.
 * https://oeis.org/A101494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle entry T(r,k) where * r = row_of_index(n), *
 *            k = n - r(r+1)/2. * The triangle is defined by: * T(n,n) = 1 *
 *            T(n,k) = Σ_{j=0}^{n-k-1} binomial(j+k, j) * T(n-1, j+k) for n >
 *            k >= 0 * (OEIS A101494)
 * Range:     u8 n <= 35, u16 n <= 66, u32 n <= 170, u64 n <= 434,
 *            u128 n <= 1128, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* row_of_index returns the row r such that
 *   r*(r+1)/2 <= n < (r+1)*(r+2)/2
 */
static inline idx_t
row_of_index(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

/* Cache of computed rows. */
static seq_t **triangle = NULL;   /* array of pointers to rows */
static idx_t rows_computed = 0;   /* number of rows stored (next free index) */

/* Ensure that rows 0..up_to inclusive are computed. */
static void
ensure_rows(idx_t up_to)
{
    /* Allocate the first row if needed. */
    if (rows_computed == 0) {
        triangle = (seq_t **)malloc(sizeof(seq_t *));
        triangle[0] = (seq_t *)malloc(sizeof(seq_t));
        triangle[0][0] = (seq_t)1;   /* T(0,0)=1 */
        rows_computed = 1;
    }

    /* Compute subsequent rows. */
    while (rows_computed <= up_to) {
        idx_t n = rows_computed;
        seq_t *row = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!row) abort();

        row[n] = (seq_t)1;            /* T(n,n)=1 */

        /* Compute entries for k = n-1 .. 0. */
        for (idx_t k = n - 1; ; --k) {
            seq_t sum = 0;
            idx_t limit = n - k - 1;

            for (idx_t j = 0; j <= limit; ++j) {
                seq_t bin = binomial(j + k, j);          /* C(j+k,j) */
                sum += bin * triangle[n - 1][j + k];     /* accumulate */
            }
            row[k] = sum;

            if (k == 0)
                break;
        }

        /* Store the new row. */
        triangle = (seq_t **)realloc(triangle,
                                     sizeof(seq_t *) * (rows_computed + 1));
        triangle[rows_computed] = row;
        ++rows_computed;
    }
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t k = n - r * (r + 1) / 2;      /* position within the row */
    ensure_rows(r);
    return triangle[r][k];
}
