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
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* Convert linear index n (1‑based) to triangle row r and column m.
   After the call r >= 1, 0 <= m < r. */
static void idx_to_row_m(idx_t n, idx_t *row, idx_t *col)
{
    /* Find smallest r s.t. r*(r+1)/2 >= n. */
    idx_t low = 1, high = n, r = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = (seq_t)mid * (mid + 1) / 2;
        if (tri >= (seq_t)n) {
            r = mid;
            if (mid == 0) break;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    *row = r;
    /* Triangular number of previous row: (r-1)*r/2. */
    seq_t tri_prev = (seq_t)(r - 1) * r / 2;
    *col = (idx_t)((seq_t)n - tri_prev - 1);   /* 0‑based column */
}
#include <stdbool.h>

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
