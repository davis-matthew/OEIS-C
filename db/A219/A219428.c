/*
 * A219428  a(n) = n - 1 - phi(n).
 * https://oeis.org/A219428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - 1 - phi(n)
 * Range:     i8 n <= 179, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 88 terms through n = 992 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

seq_t a(idx_t n) {
    /* Compute n - 1 - phi(n) using the provided phi function.
       n is an unsigned integer type (idx_t), cast to seq_t for
       signed arithmetic. */
    seq_t tot = phi(n);                /* tot = φ(n) */
    seq_t res = (seq_t)n - 1 - tot;    /* a(n) = n - 1 - φ(n) */
    return res;
}
