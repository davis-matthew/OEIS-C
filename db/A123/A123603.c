/*
 * A123603  Triangle T(n,k), 0<=k<=n, read by rows, with T(0,0) = 1, T(n,k) =
 *          0 if k<0 or if k>n, T(n,k) = T(n-1,k-1) + T(n-1,k) + T(n-2,k-2) -
 *          T(n-2,k-1) + T(n-2,k).
 * https://oeis.org/A123603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 126, u32 n <= 418, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   Triangle A123603 flattened sequence.
   a(n) is the entry T(r,k) of the triangle,
   where r is the row index and k the column index such that
   0 <= k <= r and 
        r(r+1)/2 <= n < (r+1)(r+2)/2.

   Recurrence definition of the triangle:
        T(0,0) = 1
        T(n,k) = 0  if k<0 or k>n
        T(n,k) = T(n-1,k-1) + T(n-1,k)
                    + T(n-2,k-2) - T(n-2,k-1) + T(n-2,k)

*/

#include <stdlib.h>

static seq_t **rows = NULL;          /* pointers to computed rows          */
static idx_t rows_used = 0;          /* number of rows already computed    */
static idx_t rows_alloc = 0;         /* current capacity of rows array     */

/* Ensure that rows[0..r] are computed.  The function creates rows
   incrementally, storing each complete row in the static array.       */
static void
ensure_rows(idx_t r)
{
    while (rows_used <= r) {
        idx_t nrow   = rows_used;        /* new row index */
        idx_t len    = nrow + 1;         /* number of elements in the new row */

        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) return;                /* allocation failed – abort (rare) */

        if (nrow == 0) {
            row[0] = (seq_t)1;           /* T(0,0) = 1 */
        } else if (nrow == 1) {
            row[0] = (seq_t)1;           /* T(1,0) = 1 */
            row[1] = (seq_t)1;           /* T(1,1) = 1 */
        } else {
            seq_t *prev1 = rows[nrow - 1];
            seq_t *prev2 = rows[nrow - 2];
            for (idx_t k = 0; k <= nrow; ++k) {
                seq_t val = 0;

                /* Contributions from row nrow-1 */
                if (k > 0)      val += prev1[k - 1];
                if (k < nrow)   val += prev1[k];

                /* Contributions from row nrow-2 */
                if (k > 1)      val += prev2[k - 2];
                if (k >= 1 && k < nrow) val -= prev2[k - 1];
                if (k < nrow - 1) val += prev2[k];

                row[k] = val;
            }
        }

        /* Extend the rows array if necessary */
        if (rows_used >= rows_alloc) {
            size_t new_cap = rows_alloc ? rows_alloc * 2 : 4;
            seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            if (!tmp) return;           /* allocation failed – abort */
            rows = tmp;
            rows_alloc = new_cap;
        }

        rows[rows_used++] = row;
    }
}

/* Compute a(n) as the flattened triangle entry T(r,k). */
seq_t
a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum + r + 1 <= n) {
        ++r;
        sum += r;                /* sum = r(r+1)/2 */
    }

    /* Column index k in row r. */
    idx_t k = n - sum;

    /* Compute missing rows up to r. */
    ensure_rows(r);

    /* Return the requested entry. */
    return rows[r][k];
}
