/*
 * A203431  a(n) = v(n+1)/v(n), where v=A203430.
 * https://oeis.org/A203431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (floor(3(n+1)/2) - floor(3i/2))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 290 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t top = (3 * (n + 1)) / 2;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t d = top - (3 * i) / 2;
        result *= (seq_t)d;
    }
    return result;
}
