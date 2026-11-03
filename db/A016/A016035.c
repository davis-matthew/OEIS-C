/*
 * A016035  a(n) = Sum_{j|n, 1 < j < n} phi(j). Also a(n) = n - phi(n) - 1 for
 *          n > 1.
 * https://oeis.org/A016035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n == 1, else n - phi(n) - 1
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9209 (2026-11-01)
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
    if (n == 1) {
        return 0;
    }
    return (seq_t)n - phi(n) - 1;
}
