/*
 * A127466  Triangle read by rows: A054525 * A127481 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A127466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r * φ(j) if j divides r, otherwise 0 where r is the row
 *            number containing the linear index n and j is the column within
 *            that row. The triangle is defined by T(r,j) = r * φ(j) if j | r,
 *            else 0; hence the row sum is r * Σ_{d|r} φ(d) = r². This reads
 *            the triangle row‑wise
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

static idx_t triangle_row(idx_t n) {
    /* Find the smallest r such that r*(r+1)/2 ≥ n */
    idx_t low = 1, high = n, mid;
    while (low < high) {
        mid = (low + high) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n == 0 */
    idx_t r = triangle_row(n);
    /* The first index of row r is 1 + (r-1)*r/2 */
    __int128 start = (__int128)1 + (__int128)(r - 1) * r / 2;
    idx_t j = n - (idx_t)start + 1;   /* 1‑based column number */

    if (j == 0 || j > r) return (seq_t)0;   /* safety */

    if (r % j == 0) {
        seq_t phi_j = phi(j);
        return (seq_t)r * phi_j;
    }
    return (seq_t)0;
}
