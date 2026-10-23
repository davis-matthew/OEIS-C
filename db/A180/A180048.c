/*
 * A180048  Coefficient triangle of the denominators of the (n-th convergents
 *          to) the continued fraction 1/(w+2/(w+3/(w+4/... . Conjectured to
 *          equal unsigned version of A137286.
 * https://oeis.org/A180048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The flattened sequence a(n) consists of the entries of the
 *            coefficient triangle T(n,k) of the denominators of the continued
 *            fraction 1/(w+2/(w+3/(w+4/...). T is defined by: T(0,0)=1,
 *            T(1,0)=0, T(1,1)=1, T(n,k)=T(n-1,k-1)+n*T(n-2,k) for n>=2. The
 *            index n in a(n) is obtained by stacking the rows of...
 * Range:     u8 n <= 28, u16 n <= 79, u32 n <= 192, u64 n <= 531,
 *            u128 n <= 1542, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains the index n.
       Row r starts at index r*(r+1)/2 and has r+1 entries. */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n) ++r;
    --r;                                    // r is now the correct row
    idx_t k = n - r * (r + 1) / 2;           // column within the row

    /* Memoisation table for the triangle rows. */
    static seq_t **rows = NULL;              // array of row pointers
    static idx_t rows_len = 0;               // number of rows currently stored

    /* Allocate new rows up to row r if needed. */
    if (rows_len <= r) {
        for (idx_t i = rows_len; i <= r; ++i) {
            /* Allocate storage for row i (i+1 elements). */
            seq_t *row = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
            /* Ensure the rows array can hold the new pointer. */
            if (rows == NULL) {
                rows = (seq_t **)malloc(sizeof(seq_t *));
            } else {
                rows = (seq_t **)realloc(rows,
                                          sizeof(seq_t *) * (i + 1));
            }
            if (rows == NULL || row == NULL) {
                /* In a real system you might handle allocation failure.
                   For this simple implementation we return 0. */
                return (seq_t)0;
            }
            rows[i] = row;

            /* Build the triangle entry. */
            if (i == 0) {
                row[0] = (seq_t)1;
            } else if (i == 1) {
                row[0] = (seq_t)0;
                row[1] = (seq_t)1;
            } else {
                /* Use previously computed rows i-1 and i-2. */
                seq_t *prev1 = rows[i - 1];
                seq_t *prev2 = rows[i - 2];

                /* k = 0 case: T(i,0) = i * T(i-2,0). */
                row[0] = (seq_t)i * prev2[0];

                /* Remaining coefficients. */
                for (idx_t kk = 1; kk <= i; ++kk) {
                    seq_t term = prev1[kk - 1];
                    if (kk <= i - 2) {
                        term += (seq_t)i * prev2[kk];
                    }
                    row[kk] = term;
                }
            }
        }
        rows_len = r + 1;
    }

    return rows[r][k];
}
