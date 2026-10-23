/*
 * A174190  Triangle T(n,m) = numerator of 1/n^2-1/(n-m)^2, read by rows.
 * https://oeis.org/A174190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = m(m-2n) / gcd(|m(m-2n)|, n^2*(n-m)^2), where (n,m) are
 *            obtained by reading the triangle row‑wise with rows indexed from
 *            1 and rows containing 0 ≤ m < n
 * Range:     i8 n <= 30, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   gcd, idx_to_row_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdbool.h>
#include "../utils/idx_to_row_m.h"

/* OEIS A174190 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, n>=1 in correct usage */

    idx_t r, m;
    idx_to_row_m(n, &r, &m);

    /* Numerator of 1/r^2 - 1/(r-m)^2 */
    seq_t n_seq = (seq_t)r;
    seq_t m_seq = (seq_t)m;
    seq_t N = m_seq * (m_seq - (seq_t)2 * n_seq);

    if (N == 0) return 0;

    /* Reduce N with denominator n^2 * (n-m)^2 via gcds. */
    seq_t k = N >= 0 ? N : -N;          /* absolute value of N */

    seq_t g1 = gcd(k, n_seq);
    seq_t k1 = k / g1;

    seq_t g2 = gcd(k1, n_seq);
    seq_t k2 = k1 / g2;

    seq_t d = n_seq - m_seq;

    seq_t g3 = gcd(k2, d);
    seq_t k3 = k2 / g3;

    seq_t g4 = gcd(k3, d);

    seq_t g = g1;
    g *= g2;
    g *= g3;
    g *= g4;

    return N / g;
}
