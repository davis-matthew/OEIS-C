/*
 * A128966  Triangle read by rows of coefficients of polynomials P[n](x)
 *          defined by P[0]=0, P[1]=x+1; for n >= 2,
 *          P[n]=(x+1)*P[n-1]+x*P[n-2].
 * https://oeis.org/A128966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of P[r] at index k where r is the row such
 *            that n lies in 0..(r*(r+1)/2)-1, k = n - r*(r-1)/2. Recurrence:
 *            P[0] = 0, P[1] = x+1, P[n] = (x+1)*P[n-1] + x*P[n-2] (n ≥ 2)
 * Range:     u8 n <= 39, u16 n <= 126, u32 n <= 417, u64 n <= 1507,
 *            u128 n <= 5509, big any n
 * Helpers:   find_row_and_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/find_row_and_index.h"

/* Static cache of rows of the polynomials.  rows[i] points to an array of
   length i+1 containing the coefficients of P[i]. */
static seq_t **rows = NULL;
static idx_t rows_used = 0;   /* number of rows currently allocated */

/* Ensure that rows[0..r] are computed.  */
static void ensure_rows(idx_t r)
{
    if (rows_used > r)
        return;               /* already have enough rows */

    /* Grow the rows table to size r+1 if necessary */
    if (rows == NULL) {
        rows = (seq_t **)malloc((r + 1) * sizeof(seq_t *));
    } else if (rows_used < r + 1) {
        rows = (seq_t **)realloc(rows, (r + 1) * sizeof(seq_t *));
    }

    /* Compute missing rows */
    for (idx_t i = rows_used; i <= r; ++i) {
        size_t len = (size_t)(i + 1);
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (row == NULL) {
            /* abort on allocation failure */
            exit(1);
        }

        if (i == 0) {
            /* P[0] = 0 */
            row[0] = (seq_t)0;
        } else if (i == 1) {
            /* P[1] = x + 1  -> coefficients [1,1] */
            row[0] = (seq_t)1;
            row[1] = (seq_t)1;
        } else {
            /* P[i] = (x+1)*P[i-1] + x*P[i-2] */
            seq_t *prev     = rows[i - 1];
            seq_t *prevprev = rows[i - 2];
            size_t prevlen = (size_t)i;          /* length of i-1 row   */
            size_t prevprevlen = (size_t)(i - 1); /* length of i-2 row   */

            for (size_t j = 0; j < len; ++j) {
                seq_t val = (seq_t)0;
                /* part from (x+1)*P[i-1] */
                if (j < prevlen)      /* prev[j] */
                    val += prev[j];
                if (j > 0 && j - 1 < prevlen) /* prev[j-1] * x */
                    val += prev[j - 1];
                /* part from x*P[i-2] */
                if (j > 0 && j - 1 < prevprevlen) /* prevprev[j-1] * x */
                    val += prevprev[j - 1];
                row[j] = val;
            }
        }

        rows[i] = row;
    }

    rows_used = r + 1;
}

seq_t a(idx_t n)
{
    /* Find the row and inner index for n */
    idx_t r, k;
    find_row_and_index(n, &r, &k);

    /* Ensure we have computed up to row r */
    ensure_rows(r);

    /* Return the coefficient */
    return rows[r][(size_t)k];
}
