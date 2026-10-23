/*
 * A155586  A modified Catalan sequence array.
 * https://oeis.org/A155586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = flattened triangle T(r,c), where * T(r,c) = 1 if c = 0 *
 *            Catalan(r-c) if 1 ≤ c ≤ r * The row r is chosen so that *
 *            offset_row(r) = r*(r+1)/2 ≤ n < offset_row(r+1)
 * Range:     u8 n <= 28, u16 n <= 66, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t r = 0;          /* current row number */
    idx_t offset = 0;     /* first index of current row */

    /* find row r such that n lies in row r */
    while (n >= offset + r + 1) {
        offset += r + 1; /* move past this row */
        r++;
    }

    idx_t k = n - offset;               /* column index within row r */

    if (k == 0)                         /* first element of every row */
        return (seq_t)1;

    return catalan(r - k);               /* Catalan(r-k) for k>0 */
}
