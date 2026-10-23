/*
 * A008287  Triangle of quadrinomial coefficients, row n is the sequence of
 *          coefficients of (1 + x + x^2 + x^3)^n.
 * https://oeis.org/A008287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy, nice
 * Formula:   * a(n) is the (n)-th element of the triangle of quadrinomial
 *            coefficients * T(r,k) = coefficient of x^k in (1+x+x^2+x^3)^r,
 *            enumerated row by row. * For a given index n, find the unique r
 *            such that * r*(3*r - 1)/2 <= n < (r+1)*(3*(r+1) - 1)/2 * and let
 *            k = n - r*(3*r - 1)/2. * Then * a(n) = Σ_{i=...
 * Range:     u8 n <= 56, u16 n <= 156, u32 n <= 500, u64 n <= 1861,
 *            u128 n <= 6787, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    for (;; ++r)
    {
        idx_t start = r * (3 * r - 1) / 2;
        idx_t next  = (r + 1) * (3 * (r + 1) - 1) / 2;
        if (n < next)
        {
            idx_t k = n - start;
            seq_t result = 0;
            for (idx_t i = 0; i <= k / 2; ++i)
            {
                if (i > r) break;
                idx_t j = k - 2 * i;
                if (j > r) continue;
                result += binomial(r, i) * binomial(r, j);
            }
            return result;
        }
    }
}
