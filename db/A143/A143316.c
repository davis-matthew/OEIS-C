/*
 * A143316  Triangle read by rows, A054525 * A143315, 1<=k<=n.
 * https://oeis.org/A143316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (row,col) entry of the triangle a(row,col) = 0 if
 *            row % col != 0 = phi(row/col) if col == row = 2 * phi(row/col)
 *            if col < row (phi is the Euler totient function)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column k in that row */
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t k = n - prev_tri;

    if (r % k != 0)
        return (seq_t)0;

    seq_t t = phi(r / k);   /* phi is defined for idx_t arguments */

    if (k == r)
        return t;           /* last element of each row is 1 */
    else
        return t * 2;
}
