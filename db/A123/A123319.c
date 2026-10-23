/*
 * A123319  Triangle read by rows: coefficients of polynomials p(k) = (-x + k
 *          + 1)*p(k-1), starting p(0)=1, p(1)=1-x.
 * https://oeis.org/A123319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 28, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: coefficients of polynomials
   p(k) = (-x + k + 1) * p(k-1), with p(0) = 1, p(1) = 1 - x.
   a(0) = 1; rows: {1}, {1, -1}, {3, -4, 1}, {12, -19, 8, -1}, …   */
/* This implementation builds rows successively until the row
   containing the requested index n is reached. The recurrence
   for k >= 2 is:
        b[0]   = (k+1) * c[0]
        b[j]   = (k+1) * c[j] - c[j-1]   for 1 <= j <= k-1
        b[k]   = -c[k-1]                 (last element)
   where c[0..k-1] are the coefficients of p(k-1). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate the first row (k = 0) */
    size_t cur_len = 1;
    seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
    cur[0] = (seq_t)1;

    idx_t remaining = n;   /* index to locate in the triangle */
    idx_t row = 0;         /* current row index (starting at 0) */

    /* Build rows until the remaining index falls inside the current row */
    while (remaining >= (idx_t)cur_len)
    {
        remaining -= (idx_t)cur_len;
        ++row;                    /* move to next row k = row */

        size_t next_len = row + 1;
        seq_t *next = (seq_t *)malloc(next_len * sizeof(seq_t));

        if (row == 1)     /* special initial row {1, -1} */
        {
            next[0] = (seq_t)1;
            next[1] = (seq_t)(-1);
        }
        else
        {
            /* recurrence for k >= 2 */
            seq_t coeff = (seq_t)(row + 1);   /* (k+1) */
            /* first element */
            next[0] = coeff * cur[0];
            /* middle elements */
            for (size_t j = 1; j < row; ++j)
                next[j] = coeff * cur[j] - cur[j - 1];
            /* last element */
            next[row] = -cur[row - 1];
        }

        free(cur);
        cur = next;
        cur_len = next_len;
    }

    /* remaining now indexes the element within the final row */
    seq_t result = cur[remaining];
    free(cur);
    return result;
}
