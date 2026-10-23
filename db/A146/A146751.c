/*
 * A146751  Coefficients of a new recursive polynomial: a = 11; b = 19; c =
 *          -8; r(x,n)=(a*x + b)*r(x, n - 1) + c*r(x, n - 2).
 * https://oeis.org/A146751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of the recursive polynomial r(x,n)
   defined by
        r(x,0) = 1
        r(x,1) = x + 1
        r(x,n) = (11*x + 19) * r(x,n-1) - 8 * r(x,n-2)   (n ≥ 2)

   a(n) returns the coefficient that appears at the n‑th position
   when the rows of all r(x,k) (for k = 0,1,2,…) are concatenated
   row by row.  The first values are
        1, 1, 1, 11, 30, 11, 201, 683, 539, 121, …          */

#include <stdlib.h>

/* static storage for the computed rows of coefficients */
static seq_t **coeff_rows = NULL;
static idx_t   rows_count = 0;   /* number of rows already stored */

/* ------------------------------------------------------------------ */
/* ensure_rows(up_to)   –  grow the triangular array until row 'up_to' is available   */
static void ensure_rows(idx_t up_to)
{
    if (rows_count > up_to) return;            /* nothing to do */

    /* enlarge the pointer array */
    seq_t **new_rows = (seq_t **)malloc((up_to + 1) * sizeof(seq_t *));
    if (!new_rows) exit(1); /* allocation failure */
    for (idx_t i = 0; i < rows_count; ++i) new_rows[i] = coeff_rows[i];

    /* compute new rows */
    for (idx_t i = rows_count; i <= up_to; ++i) {
        seq_t *row = (seq_t *)malloc((i + 1) * sizeof(seq_t));
        if (!row) exit(1);                    /* allocation failure */

        if (i == 0) {                         /* r(x,0) = 1 */
            row[0] = (seq_t)1;
        } else if (i == 1) {                  /* r(x,1) = x + 1 */
            row[0] = (seq_t)1;
            row[1] = (seq_t)1;
        } else {
            seq_t *prev1 = coeff_rows[i-1];   /* r(x,i-1)   */
            seq_t *prev2 = coeff_rows[i-2];   /* r(x,i-2)   */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t shift = (k > 0) ? prev1[k-1] : (seq_t)0;
                seq_t part1 = (seq_t)11 * shift;
                seq_t part2 = (k <= i-1) ? (seq_t)19 * prev1[k] : (seq_t)0;
                seq_t part3 = (k <= i-2) ? (seq_t)8  * prev2[k] : (seq_t)0;

                seq_t val = part1 + part2;    /* a·shift + b·prev1  */
                val -= part3;                 /* subtract 8·prev2    */
                row[k] = val;
            }
        }

        new_rows[i] = row;
    }

    free(coeff_rows);
    coeff_rows = new_rows;
    rows_count = up_to + 1;
}

/* ------------------------------------------------------------------ */
/* a(n) – main routine requested by the unit tests                         */
seq_t a(idx_t n)
{
    /* locate the row that contains the required coefficient */
    idx_t   total = 0;          /* number of elements before the current row   */
    idx_t   r = 0;
    while (total + (r + 1) <= n) {   /* advance until n is inside row r   */
        total += r + 1;
        ++r;
    }
    idx_t k = n - total;            /* column inside row r                     */

    ensure_rows(r);                 /* guarantee that row r exists             */

    return coeff_rows[r][k];
}
