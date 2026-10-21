/*
 * A078391  Triangle read by rows: T(n,k) = Catalan(k)*Catalan(n-k).
 * https://oeis.org/A078391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = T(r,k) where r = largest integer with r(r+1)/2 ≤ n, k = n
 *            - r(r+1)/2, and T(r,k) = Catalan(k)·Catalan(r-k) (row‑by‑row
 *            flattening of the triangle)
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    /* find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;

    seq_t left  = catalan(k);
    seq_t right = catalan(r - k);
    return left * right;
}
