/*
 * A127952  Triangle read by rows, T(n,k) = (n+1)*binomial(n-1,k-1).
 * https://oeis.org/A127952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r+1) * binomial(r-1, c-1) where r is the row index of n
 *            and c is the column index within that row (0‑based). The
 *            triangle is read by rows, with offset_row(r) giving the linear
 *            index of the first element of row r. For r = 0 the single value
 *            is 1, and for r > 0 and c = 0 all entries are 0
 * Range:     u8 n <= 39, u16 n <= 142, u32 n <= 510, u64 n <= 2044,
 *            u128 n <= 5049, big any n
 * Helpers:   binomial, offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/offset_row.h"

seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) <= n < offset_row(r+1). */
    idx_t r = 0;
    while (offset_row(r + 1) <= n)
        ++r;

    /* Column index within the row. */
    idx_t c = n - offset_row(r);

    if (r == 0)
        return (seq_t)1;        /* row 0 has single entry 1 */

    if (c == 0)
        return (seq_t)0;        /* first element of any row r>0 is 0 */

    /* General case: (r+1) * binomial(r-1, c-1). */
    seq_t bin = binomial(r - 1, c - 1);
    return (seq_t)(r + 1) * bin;
}
