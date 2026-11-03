/*
 * A101124  Number triangle associated to Chebyshev polynomials of first kind.
 * https://oeis.org/A101124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is obtained by reading the triangle * S(r,c) = T(r-c,c) (k
 *            = c) * in row-major order. The row index r is the * largest
 *            integer with r*(r+1)/2 <= n. * Let c = n - r*(r+1)/2. If c == r
 *            then * a(n) = 1, otherwise a(n) = T_{r-c}(c), * where T_m(x) is
 *            the Chebyshev polynomial of the first kind, *...
 * Range:     i8 n <= 29, i16 n <= 57, i32 n <= 123, i64 n <= 305,
 *            i128 n <= 828, big any n
 * Validated: standard, pass, 92 terms through n = 9470 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t chebyshev(idx_t m, idx_t k)
{
    if (m == 0) return (seq_t)1;
    if (m == 1) return (seq_t)k;

    seq_t prevprev = 1;
    seq_t prev     = (seq_t)k;
    seq_t curr;

    for (idx_t i = 2; i <= m; ++i) {
        curr = prev * (((seq_t)2) * (seq_t)k) - prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;
}

seq_t a(idx_t n)
{
    /* find row r: largest r with r*(r+1)/2 <= n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t offset = n - r * (r + 1) / 2;
    idx_t c = offset;

    if (c == r) return (seq_t)1;

    idx_t m = r - c;
    return chebyshev(m, c);
}
