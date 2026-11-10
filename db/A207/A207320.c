/*
 * A207320  a(n) = Sum_{k=0..n-1} (-1)^k*k^2*A000172(k).
 * https://oeis.org/A207320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n-1} (-1)^k*k^2*F(k), F = A000172 (Franel
 *            numbers), computed by (m+1)^2*F(m+1) = (7*m^2+7*m+2)*F(m) +
 *            8*m^2*F(m-1), F(0)=1, F(1)=2
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev = 1, cur = 2, s = 0, k2, m, next;
    idx_t k;
    for (k = 1; k < n; k++) {
        k2 = (seq_t)k * (seq_t)k;
        if (k & 1)
            s -= k2 * cur;
        else
            s += k2 * cur;
        if (k + 1 < n) {
            m = (seq_t)k;
            next = ((7 * m * m + 7 * m + 2) * cur + 8 * m * m * prev)
                   / ((m + 1) * (m + 1));
            prev = cur;
            cur = next;
        }
    }
    return s;
}
