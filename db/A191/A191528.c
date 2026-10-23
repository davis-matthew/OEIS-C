/*
 * A191528  Triangle read by rows: T(n,k) is the number of left factors of
 *          Dyck paths of length n that have k returns to the axis.
 * https://oeis.org/A191528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r - k - 1, ceil(r/2) - 1) where r is the row
 *            containing the nth triangle entry and k is the column in that
 *            row. For r = 0 the value is 1
 * Range:     u8 n <= 24, u16 n <= 109, u32 n <= 341, u64 n <= 1224,
 *            u128 n <= 2600, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t row = 0;         /* current row index being examined */
    idx_t cum = 0;         /* cumulative number of entries before current row */

    /* Find the row that contains the nth entry.
       Each row r has 1 + floor(r/2) entries. */
    while (1)
    {
        idx_t len = 1 + row / 2;      /* length of row 'row' */
        if (cum + len > n)            /* n lies in this row */
            break;
        cum += len;
        ++row;
    }

    /* Column index within the row */
    idx_t k = n - cum;

    /* Special case for the very first row (row = 0). */
    if (row == 0)
        return 1;

    /* Binomial parameters:
       n1 = row - k - 1,
       k1 = ceil(row/2) - 1 = (row + 1)/2 - 1  (integer arithmetic). */
    idx_t n1 = row - k - 1;
    idx_t k1 = (row + 1) / 2 - 1;

    return binomial(n1, k1);
}
