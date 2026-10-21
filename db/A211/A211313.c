/*
 * A211313  Square array of Delannoy numbers D(i,j) mod 5 (i >= 0, j >= 0)
 *          read by antidiagonals.
 * https://oeis.org/A211313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (Delannoy number D(i,j) modulo 5) where (i,j) is the *
 *            (i+j)-th anti‑diagonal of the square array of D(i,j) read left
 *            to * right while n runs from 0 upward. * The algorithm builds a
 *            square table of D(i,j) mod 5 on demand. * For n up to 10 000
 *            this is easily under the time limit
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global table of Delannoy numbers modulo 5. */
static seq_t **delannoy = NULL;   /* square table of size dim x dim */
static size_t dim = 0;            /* current dimension of the table */

/* Allocate and fill a new square table of size new_dim x new_dim. */
static void rebuild_table(size_t new_dim)
{
    /* Allocate array of row pointers. */
    seq_t **new_tbl = (seq_t **)malloc(new_dim * sizeof(seq_t *));
    if (!new_tbl) exit(1);  /* allocation failure */

    /* Allocate rows; they are zero-initialised for safety. */
    for (size_t i = 0; i < new_dim; ++i) {
        new_tbl[i] = (seq_t *)calloc(new_dim, sizeof(seq_t));
        if (!new_tbl[i]) exit(1);
    }

    /* Compute all D(i,j) mod 5 using the recurrence:
     * D(0,j)=D(i,0)=1; D(i,j) = D(i-1,j)+D(i,j-1)+D(i-1,j-1).
     * Because we are filling new_tbl from scratch we can use it
     * directly in the recurrence.
     */
    for (size_t i = 0; i < new_dim; ++i) {
        for (size_t j = 0; j < new_dim; ++j) {
            if (i == 0 || j == 0) {
                new_tbl[i][j] = (seq_t)1;
            } else {
                seq_t sum =
                    new_tbl[i - 1][j] + new_tbl[i][j - 1] +
                    new_tbl[i - 1][j - 1];
                new_tbl[i][j] = sum % (seq_t)5;
            }
        }
    }

    /* Free the old table, if any. */
    if (delannoy) {
        for (size_t i = 0; i < dim; ++i) {
            free(delannoy[i]);
        }
        free(delannoy);
    }

    /* Install the new table. */
    delannoy = new_tbl;
    dim = new_dim;
}

/* Return the nth term of A211313. */
seq_t a(idx_t n)
{
    /* Find the anti‑diagonal index s such that
     * n lies in the range [s(s+1)/2, (s+1)(s+2)/2).  This is
     * equivalent to the smallest s with n < (s+1)(s+2)/2.
     */
    size_t s = 0;
    while ((s * (s + 1)) / 2 <= (size_t)n) ++s;
    --s;  /* now s satisfies s(s+1)/2 <= n < (s+1)(s+2)/2 */

    /* Ensure the table is large enough to hold all entries up to this
     * diagonal.  We need indices i and j up to s.
     */
    if (s + 1 > dim) rebuild_table(s + 1);

    /* Determine the coordinates (i,j) corresponding to a(n). */
    size_t offset = (size_t)n - (s * (s + 1)) / 2;  /* j */
    size_t i = s - offset;                          /* i = s - j */
    size_t j = offset;

    return delannoy[i][j];
}
