/*
 * A086196  Incorrect version of A380267.
 * https://oeis.org/A086196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = denominator((1 + Sum_{k=1..n} A051904(k))/(n+1))
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A086196_minexp(idx_t n)
{
    if (n == 1) return 0;
    idx_t min_exp = 0;
    idx_t temp = n;
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) { temp >>= 1; ++e; }
        min_exp = e;
    }
    for (idx_t p = 3; (unsigned __int128)p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            if (min_exp == 0 || e < min_exp) min_exp = e;
        }
    }
    if (temp > 1) {
        if (min_exp == 0 || 1 < min_exp) min_exp = 1;
    }
    return min_exp;
}

seq_t a(idx_t n)
{
    idx_t s = 1;
    for (idx_t k = 1; k <= n; k++)
        s += A086196_minexp(k);
    idx_t d = n + 1;
    idx_t x = s, y = d;
    while (y) { idx_t t = x % y; x = y; y = t; }
    return (seq_t)(d / x);
}
