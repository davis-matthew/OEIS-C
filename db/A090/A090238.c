/*
 * A090238  Triangle T(n, k) read by rows. T(n, k) is the number of lists of k
 *          unlabeled permutations whose total length is n.
 * https://oeis.org/A090238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: A090238 is the triangle T(n,k) read by rows where
 *
 *   T(0,0) = 1
 *   T(n,0) = 0  (n > 0)
 *   T(0,k) = 0  (k > 0)
 *   T(n,k) = T(n-1,k-1) + ((n+k-1)/k) * T(n-1,k)  (n > 0, k > 0)
 *
 * The sequence a(n) enumerates the entries of the triangle in row‑wise
 * order.  That is,  a(n) = T(row, col) where
 *   row  = maximal integer with row*(row+1)/2 <= n
 *   col  = n - row*(row+1)/2 .
 *
 * This function computes a(n) for any n >= 0 using the recurrence.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial first element */
    if (n == 0)
        return (seq_t)1;

    /* Determine the row that contains the n‑th entry in row‑wise order */
    idx_t row = 0;
    while (((row + 1) * (row + 2)) / 2 <= n)
        ++row;          /* now row*(row+1)/2 <= n < (row+1)*(row+2)/2 */

    /* Index of the entry within this row */
    idx_t start = (row * (row + 1)) / 2;
    idx_t col   = n - start;   /* 0 <= col <= row */

    /* Allocate two row buffers; we reuse them for each iteration   */
    seq_t *prev = NULL;  /* previous row (row index r-1) */
    seq_t *curr = NULL;  /* current row      (row index r)   */

    /* Build the triangle up to the desired row */
    for (idx_t r = 0; r <= row; ++r)
    {
        /* allocate array for current row of length r+1 */
        curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (!curr) return (seq_t)0;   /* memory failure – return 0 */

        /* first element of every row (except the very first) is 0 */
        curr[0] = (idx_t)(r == 0 ? 1 : 0);  /* row 0 has T(0,0)=1, others T(n,0)=0 */

        /* compute remaining elements using the recurrence */
        for (idx_t k = 1; k <= r; ++k)
        {
            seq_t term = (seq_t)0;
            if (k <= r - 1)   /* prev[k] exists only when k <= r-1 */
            {
                /* term = ((r + k - 1) * T(r-1,k)) / k */
                term = ( ( (seq_t)(r + k - 1) ) * prev[k] ) / (seq_t)k;
            }
            curr[k] = prev[k - 1] + term;
        }

        /* free the previous row after it has been used */
        if (prev) free(prev);
        prev = curr;  /* move current row to previous for next iteration */
    }

    /* prev now points to the desired row (row == desired row) */
    seq_t result = prev[col];
    free(prev);          /* free the last row buffer */
    return result;
}
