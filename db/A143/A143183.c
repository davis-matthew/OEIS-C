/*
 * A143183  Triangle T(n,k) = 1 + (2+n)*abs(n-2*k), read by rows, for 0 <= k
 *          <= n.
 * https://oeis.org/A143183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the linear write‑out, by rows, of the triangle T(r,k) =
 *            1 + (2+r)*abs(r-2*k) (0 ≤ k ≤ r)
 * Range:     u8 n <= 119, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r containing index n.
       The offset of row r is r*(r+1)/2.  */
    idx_t r          = 0;
    idx_t offset     = 0;                     /* r*(r+1)/2 for current r */
    idx_t nextOffset = 1;                     /* (r+1)*(r+2)/2 */

    /* Increment r until the next row would exceed n. */
    while (nextOffset <= n) {
        r++;
        offset = nextOffset;
        nextOffset = (r + 1) * (r + 2) / 2;
    }

    /* Column index within the row. */
    idx_t k = n - offset;

    /* Compute |r - 2*k| without turning on floating point. */
    idx_t diff = (r >= 2 * k) ? (r - 2 * k) : (2 * k - r);

    /* Evaluate T(r,k) in seq_t arithmetic. */
    seq_t term = (seq_t)1 + (((seq_t)2 + (seq_t)r) * (seq_t)diff);

    return term;
}
