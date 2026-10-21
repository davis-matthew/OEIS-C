/*
 * A207645  Triangle where T(n,k) = Product_{j=1..k} floor(n/j - 1), as read
 *          by rows n>=0, columns k=0..[n/2].
 * https://oeis.org/A207645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, tabf
 * Formula:   a(n) = Π_{j=1..k} floor(r/j - 1) where r is the unique * integer
 *            with Σ_{i=0}^{r-1} (⌊i/2⌋+1) ≤ n < Σ_{i=0}^{r} (⌊i/2⌋+1), * and
 *            k = n - Σ_{i=0}^{r-1} (⌊i/2⌋+1). The sequence is the flattening
 *            * of triangles T(r,k) defined by T(r,0)=1 and *
 *            T(r,k)=Π_{j=1..k} floor(r/j-1) for 0<k≤⌊r/2⌋
 * Range:     u8 n <= 45, u16 n <= 161, u32 n <= 474, u64 n <= 1295,
 *            u128 n <= 1295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t row = 0;          /* current row number */
    idx_t idx = n;          /* remaining position in the flattening */
    while (1) {
        /* number of elements in this row: floor(row/2)+1 */
        idx_t row_len = row / 2 + 1;
        if (idx < row_len) {          /* desired element is in this row */
            idx_t k = idx;            /* column index */
            /* T(row,0) = 1 */
            if (k == 0) return (seq_t)1;
            seq_t prod = 1;
            for (idx_t j = 1; j <= k; ++j) {
                /* floor(row/j - 1) = (row/j) - 1 for positive integers */
                seq_t term = (seq_t)(row / j) - 1;
                prod *= term;
            }
            return prod;
        }
        /* skip this row and move to next */
        idx -= row_len;
        ++row;
    }
}
