/*
 * A120073  Denominator triangle for hydrogen spectrum rationals.
 * https://oeis.org/A120073
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) = denominator of r(m,n) where r(m,n) = 1/n^2 - 1/m^2, and
 *            (m,n) are the coordinates of the n‑th term in the antidiagonal
 *            enumeration of the hydrogen‑spectrum rational triangle (offset
 *            2)
 * Range:     u8 n <= 10, u16 n <= 136, u32 n <= 1226, u64 n <= 1226,
 *            u128 n <= 1226, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* If n is below the offset, return 0 (not used by the test harness). */
    if (n < 2)
        return 0;

    /* k counts from 0: a(2) corresponds to k=0. */
    idx_t k = n - 2;

    /* Find the row m such that C(m-1) <= k < C(m),
       where C(m) = (m-1)*m/2 is the cumulative count up to row m. */
    idx_t m = 2;
    while ((m - 1) * m / 2 <= k)
        ++m;

    /* Compute index within the row (0‑based). */
    idx_t prevC = (m - 2) * (m - 1) / 2;
    idx_t t = k - prevC;              /* 0‑based column */
    idx_t nn = t + 1;                  /* 1‑based n */

    /* Convert to seq_t for arithmetic. */
    seq_t m_seq = (seq_t)m;
    seq_t n_seq = (seq_t)nn;

    /* r(m,n) = (m-n)*(m+n) / (m^2 * n^2)  */
    seq_t num = (m_seq - n_seq) * (m_seq + n_seq);
    seq_t denom = (m_seq * m_seq) * (n_seq * n_seq);

    /* Reduce the fraction to lowest terms. */
    seq_t g = gcd(denom, num);
    if (g != 0)
        denom /= g;

    return denom;
}
