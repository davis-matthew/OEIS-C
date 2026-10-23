/*
 * A181543  Triangle of cubed binomial coefficients, T(n,k) = C(n,k)^3, read
 *          by rows.
 * https://oeis.org/A181543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, k)^3, where r is the largest integer with *
 *            r(r+1)/2 ≤ n and k = n – r(r+1)/2. This reads the triangle of
 *            cubed * binomial coefficients T(n,k) = C(n,k)^3 row by row
 *            starting at offset 0
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 96, u64 n <= 311,
 *            u128 n <= 1102, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t tri = 0;          /* tri = T_r = r(r+1)/2 */
    while (tri + (r + 1) <= n)
    {
        r++;
        tri += r;
    }
    idx_t k = n - tri;
    seq_t c = binomial(r, k);
    return c * c * c;
}
