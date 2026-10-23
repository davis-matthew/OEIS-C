/*
 * A134634  Triangle formed by Pascal's rule with borders = A000108.
 * https://oeis.org/A134634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A134634: Triangle formed by Pascal's rule with borders = A000108
   a(n) = T(r,k) where n = offset_row(r)+k and
   T(r,0)=T(r,r)=Catalan(r),  T(r,k)=T(r-1,k)+T(r-1,k-1) for 0<k<r
   offset_row(r) = r*(r+1)/2
*/

#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) <= n < offset_row(r+1) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;

    seq_t *prev = NULL;      /* previous row (size r) */
    size_t prev_len = 0;

    for (idx_t row = 0; row <= r; ++row) {
        size_t len = row + 1;
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * len);
        if (!curr) abort(); /* allocation failure */

        /* Borders */
        curr[0] = catalan(row);
        curr[row] = catalan(row);

        /* Interior entries */
        for (size_t j = 1; j < row; ++j) {
            curr[j] = prev[j] + prev[j - 1];
        }

        if (row == r) {
            seq_t result = curr[k];
            free(prev);
            free(curr);
            return result;
        }

        free(prev);
        prev = curr;
        prev_len = len;
    }

    /* Should never reach here */
    return 0;
}
