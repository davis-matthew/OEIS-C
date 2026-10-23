/*
 * A178252  Triangle T(n,m) read by rows: the numerator of the coefficient
 *          [x^m] of the umbral inverse Bernoulli polynomials B^{-1}(n,x), 0
 *          <= m <= n.
 * https://oeis.org/A178252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = binom(r+1,k) / gcd(binomial(r+1,k), r+1) Where r is the
 *            row index satisfying r·(r+1)/2 ≤ n < (r+1)(r+2)/2, and k = n −
 *            r·(r+1)/2
 * Range:     u8 n <= 31, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + (r + 1) <= n)
    {
        cum += r + 1;
        r++;
    }
    idx_t k = n - cum;
    seq_t bin = binom(r + 1, k);
    seq_t denom = (seq_t)(r + 1);
    seq_t g = gcd(bin, denom);
    return bin / g;
}
