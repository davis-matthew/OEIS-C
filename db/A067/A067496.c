/*
 * A067496  Duplicate of A054565.
 * https://oeis.org/A067496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th k >= 1 such that 3^k has leading digit 3, i.e.
 *            frac(k*log10(3)) lies in [log10(3), log10(4)); found by linear
 *            search
 * Range:     i8 n <= 16, i16 n <= 31, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    const long double L3 = log10l(3.0L);
    const long double L4 = log10l(4.0L);
    idx_t count = 0;
    idx_t e = 0;
    if (n == 0) return (seq_t)0;
    while (count < n) {
        ++e;
        long double x = (long double)e * L3;
        long double frac = x - floorl(x);
        if (frac >= L3 && frac < L4) ++count;
    }
    return (seq_t)e;
}
