/*
 * A164295  Triangle T(n,k) read by rows: sum of the triangles A054521 and
 *          A051731.
 * https://oeis.org/A164295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where (r,k) are the coordinates of n in the *
 *            triangular array read by rows. The entry T(r,k) is defined as *
 *            T(r,k) = 2 if k == 1 * T(r,k) = 0 if gcd(r,k) > 1 and r % k != 0
 *            * T(r,k) = 1 otherwise * * This implements the OEIS triangle
 *            A164295
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   gcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_idx_t.h"

seq_t a(idx_t n)
{
    /* Find triangular index r such that r*(r+1)/2 >= n > (r-1)*r/2 */
    idx_t low = 1;
    idx_t high = n;          /* r is never larger than n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute k = n - ((r-1)*r/2) */
    __int128 tri_prev128 = (__int128)(r - 1) * r / 2;
    idx_t tri_prev = (idx_t)tri_prev128;
    idx_t k = n - tri_prev;

    /* Compute the triangle entry */
    if (k == 1)
        return (seq_t)2;

    idx_t g = gcd_idx_t(r, k);
    if (g > 1 && (r % k != 0))
        return (seq_t)0;

    return (seq_t)1;
}
