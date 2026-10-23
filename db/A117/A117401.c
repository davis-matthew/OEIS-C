/*
 * A117401  Triangle T(n,k) = 2^(k*(n-k)), read by rows.
 * https://oeis.org/A117401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(k*(r-k)) where r is the row index such that r(r+1)/2 ≤
 *            n < (r+1)(r+2)/2 and k = n - r(r+1)/2
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 81, u64 n <= 143,
 *            u128 n <= 285, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r where the triangular number T(r) = r(r+1)/2 ≤ n. */
    idx_t r     = 0;           /* current row */
    idx_t tri   = 0;           /* T(r) */
    while (1)
    {
        idx_t next_tri = tri + r + 1;   /* T(r+1) */
        if (n < next_tri)
            break;
        tri = next_tri;
        ++r;
    }

    /* Position k inside the row: n = T(r) + k  →  k = n - T(r)  */
    idx_t k = n - tri;

    /* Exponent for the power of two: exp = k * (r - k)  */
    idx_t exp = k * (r - k);

    /* Return 2^exp using the provided pow2 utility */
    return pow2(exp);
}
