/*
 * A098495  Array T(r,c) read by antidiagonals: values of triangle A098493
 *          interpreted as polynomials, r >= 0.
 * https://oeis.org/A098495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(r,c) = (1-r)*T(r,c-1) - T(r,c-2), T(r,0)=1, T(r,1)=-r; a(n) =
 *            T(r,c) read by antidiagonals (r descending: r=d-k, c=k,
 *            n=d(d+1)/2+k)
 * Range:     i8 n <= 47, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 81 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    idx_t k = n - d * (d + 1) / 2;
    idx_t r = d - k;
    idx_t c = k;

    seq_t x0 = 1;
    if (c == 0) return x0;
    seq_t x1 = -(seq_t)r;
    seq_t coef = (seq_t)1 - (seq_t)r;
    for (idx_t i = 2; i <= c; i++) {
        seq_t x2 = coef * x1 - x0;
        x0 = x1;
        x1 = x2;
    }
    return x1;
}
