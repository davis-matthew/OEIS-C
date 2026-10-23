/*
 * A124895  Triangle read by rows, 1 <= k <= n: T(n,k) = mu(n^2 + k^2) with mu
 *          = A008683.
 * https://oeis.org/A124895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = μ( n^2 + k^2 ), where k is the column index in the *
 *            triangle read by rows (offset 1). T(r,k) = μ(r^2 + k^2), r ≥ 1,
 *            * 1 ≤ k ≤ r, and a(n) is the linear reading of the triangle
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* offset is 1; n ≥ 1 for valid terms. */
    if (n == 0)
        return 0;

    /* Find the row r such that C(r-1) < n ≤ C(r), where
     * C(r) = r*(r+1)/2 is the total number of terms up to row r.
     * Since r ≤ 141 for n ≤ 10000, a simple linear search is fast. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column index k in this row (1‑based). */
    idx_t k = n - (r - 1) * r / 2;

    /* Compute μ(r² + k²).  The value fits comfortably in 64 bits
     * for the required range, and μ is already provided in utils. */
    idx_t m = r * r + k * k;
    return mobius(m);
}
