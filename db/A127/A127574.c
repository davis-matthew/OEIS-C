/*
 * A127574  Triangle T(n,k) = k*sigma(n) if k divides n, else 0.
 * https://oeis.org/A127574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = smallest integer with r(r+1)/2 ≥ n, c =
 *            n − r(r−1)/2, T(r,c) = (c divides r) ? c * sigma(r) : 0
 * Range:     u8 n <= 77, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Find the row r such that triangular(r-1) < n ≤ triangular(r) */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n) {
        ++r;
    }
    /* Column within the row */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t c = n - prev_tri;           /* 1 ≤ c ≤ r */

    /* Return 0 if k does not divide n, else k * sigma(n) */
    if ((r % c) != 0) {
        return (seq_t)0;
    }
    return (seq_t)c * sigma_of(r);
}
