/*
 * A120072  Numerator triangle for hydrogen spectrum rationals.
 * https://oeis.org/A120072
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) = numerator((1/n^2)-(1/m^2)) where m is chosen so that the
 *            term a(n) lies at position n in the triangle r(m,k)=1/k^2-1/m^2
 *            (m>=2, k=1..m-1)
 * Range:     u8 n <= 120, u16 n <= 1226, u32 n <= 1226, u64 n <= 1226,
 *            u128 n <= 1226, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* t is the zero-based position of the term in the flattened triangle
       starting from the first term (a(2)). */
    idx_t t = n - 1;

    /* Find the row m such that t <= (m-1)m/2. */
    idx_t m = 2;
    idx_t cumulative = 0;
    for (;; ++m) {
        cumulative += m - 1;
        if (cumulative >= t) break;
    }

    /* Position within the row (zero-based). */
    idx_t prev = cumulative - (m - 1);
    idx_t pos = t - prev - 1;
    idx_t n_val = pos + 1;

    /* Compute squares. */
    seq_t m_sq = (seq_t)m * m;
    seq_t n_sq = (seq_t)n_val * n_val;

    /* Numerator before reduction: m^2 - n^2. */
    seq_t diff = m_sq - n_sq;

    /* Compute the gcd of diff with the denominator m^2*n^2
       without forming the large product. */
    seq_t g1 = gcd(diff, m_sq);
    seq_t diff1 = diff / g1;
    seq_t g2 = gcd(diff1, n_sq);
    seq_t denom_gcd = g1 * g2;

    return diff / denom_gcd;
}
