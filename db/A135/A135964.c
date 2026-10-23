/*
 * A135964  a(n) = binomial(floor(n*sqrt(2)),n) for n>=0.
 * https://oeis.org/A135964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(floor(n*sqrt(2)), n)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 28, u64 n <= 55, u128 n <= 107,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/gcd.h"

/* Compute binomial coefficient C(m,k) using an
   exact multiplicative algorithm that reduces
   at each step to avoid overflow and preserve
   exactness. */
static seq_t binomial_compute(seq_t m, seq_t n)
{
    /* n <= m for the use in this sequence */
    seq_t k = n;
    seq_t m_minus_k = m - k;
    if (k > m_minus_k) {
        k = m_minus_k;
    }

    seq_t res = 1;
    idx_t steps = (idx_t)k;

    for (idx_t i = 1; i <= steps; ++i) {
        seq_t numerator   = m - k + (seq_t)i;
        seq_t denominator = (seq_t)i;

        /* Reduce numerator and denominator by gcd */
        seq_t g1 = gcd(numerator, denominator);
        if (g1 > 1) {
            numerator   /= g1;
            denominator /= g1;
        }

        /* Further reduce denominator with current result */
        seq_t g2 = gcd(res, denominator);
        if (g2 > 1) {
            res        /= g2;
            denominator /= g2;
        }

        /* denominator should now be 1 */
        res *= numerator;
    }

    return res;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t = (seq_t)2 * N * N;          /* 2 * n^2   */
    seq_t m = floor_sqrt(t);             /* floor(n*sqrt(2)) */
    return binomial_compute(m, N);
}
