/*
 * A124038  Triangle read by rows: T(n, k) = T(n-1, k-1) - T(n-2, k), with
 *          T(n, n) = 1, T(n, n-1) = -2.
 * https://oeis.org/A124038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the triangle T(n,k) row‑by‑row,
 *            where T(n,k) satisfies T(n,k) = T(n-1,k-1) - T(n-2,k), with
 *            T(n,n)=1, T(n,n-1)=-2. Boundary values for k<0 or k>n are
 *            treated as 0. The function computes a(n) for any n >= 0
 * Range:     i8 n <= 94, i16 n <= 334, i32 n <= 1244, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the very first term directly. */
    if (n == 0)
        return 1;

    /* Find the row r containing the term a(n). */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within that row. */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Allocate the first two rows of the triangle. */
    seq_t *row_n_minus2 = (seq_t *)malloc(1 * sizeof(seq_t));  /* row 0 */
    if (!row_n_minus2) return 0;
    row_n_minus2[0] = 1;

    seq_t *row_n_minus1 = (seq_t *)malloc(2 * sizeof(seq_t));  /* row 1 */
    if (!row_n_minus1) {
        free(row_n_minus2);
        return 0;
    }
    row_n_minus1[0] = -2;
    row_n_minus1[1] = 1;

    if (r == 1) {                /* The required term is already computed. */
        seq_t result = row_n_minus1[k];
        free(row_n_minus1);
        free(row_n_minus2);
        return result;
    }

    /* Compute subsequent rows up to row r. */
    seq_t *curr = NULL;
    for (idx_t row = 2; row <= r; ++row) {
        idx_t sz = row + 1;
        curr = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!curr) {
            free(row_n_minus1);
            free(row_n_minus2);
            return 0;
        }

        for (idx_t col = 0; col < sz; ++col) {
            if (col == row) {
                curr[col] = 1;                     /* T(n,n) = 1 */
            } else if (col == row - 1) {
                curr[col] = -2;                    /* T(n,n-1) = -2 */
            } else {
                /* Recurrence: T(n,k) = T(n-1,k-1) - T(n-2,k) */
                seq_t left = (col > 0) ? row_n_minus1[col - 1] : 0;
                seq_t down = (col <= row - 2) ? row_n_minus2[col] : 0;
                curr[col] = left - down;
            }
        }

        free(row_n_minus2);          /* free the oldest row */
        row_n_minus2 = row_n_minus1;
        row_n_minus1 = curr;
    }

    /* Retrieve the requested element and clean up. */
    seq_t result = row_n_minus1[k];
    free(row_n_minus1);
    free(row_n_minus2);
    return result;
}
