/*
 * A178395  Triangle T(n,m) read by rows: the numerator of the coefficient
 *          [x^m] of the inverse Euler polynomial E^{-1}(n,x), 0 <= m <= n.
 * https://oeis.org/A178395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) is the row‑major enumeration of the triangle T(r,m) =
 *            binom(r,m) if binom(r,m) is odd, otherwise binom(r,m)/2. Here r
 *            = floor((sqrt(8*n+1)-1)/2) and m = n - r*(r+1)/2
 * Range:     u8 n <= 39, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Offset of the first element of row r */
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t m = n - (idx_t)start;          /* column within the row */

    /* Compute the binomial coefficient C(r,m) exactly. */
    seq_t val = binom(r, m);

    /* If the binomial coefficient is even, divide by 2 to get the numerator
       of (C(r,m)/2).  If it is odd, the numerator is just C(r,m). */
    if ((val & 1) == 0)
        val >>= 1;

    return val;
}
