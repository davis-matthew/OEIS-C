/*
 * A080046  Multiplicative Pascal triangle, read by rows: T(n,1)=T(n,n)=n and
 *          T(n,k) = T(n-1,k-1) * T(n-1,k).
 * https://oeis.org/A080046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 23, u32 n <= 31, u64 n <= 39, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the linearisation of the multiplicative Pascal triangle T(n,k)
 * defined by:
 *   T(n,1) = T(n,n) = n
 *   T(n,k) = T(n-1,k-1) * T(n-1,k)        for 1 < k < n
 * The sequence is read row‑by‑row, left‑to‑right, with a(1) = 1.
 *
 * The implementation builds the triangle row by row, keeping only the
 * previous row in memory.  It runs in O(n) time and O(r) space,
 * where r ≈ √(2n) is the row containing the n‑th term.
 */

#include <stddef.h> /* for size_t */

#define MAX_ROW 2048  /* safely larger than needed for n ≈ 10,000 */

seq_t a(idx_t n)
{
    /* storage for the previous and current rows (1‑based indexing) */
    static seq_t prev[MAX_ROW];
    static seq_t curr[MAX_ROW];

    /* a(1) is the only special case */
    if (n == 1ULL) {
        return (seq_t)1;
    }

    /* initialise the first row */
    idx_t row = 1;
    prev[1] = (seq_t)1;
    idx_t rowsum = 1;          /* number of terms generated so far   */

    /* generate rows until the n‑th term is reached           */
    for (row = 2; ; ++row) {
        curr[1] = (seq_t)row;          /* left boundary    */
        curr[row] = (seq_t)row;        /* right boundary   */

        /* interior values: product of two adjacent values in previous row */
        for (idx_t k = 2; k < row; ++k) {
            curr[k] = prev[k - 1] * prev[k];
        }

        rowsum += row;                  /* new total count of terms   */

        if (n <= rowsum) {              /* the desired term is in this row   */
            idx_t kpos = n - (rowsum - row); /* position within the row (1‑based) */
            return curr[kpos];
        }

        /* copy current row to prev for the next iteration */
        for (idx_t i = 1; i <= row; ++i) {
            prev[i] = curr[i];
        }
    }
}
