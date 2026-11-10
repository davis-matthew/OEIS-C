/*
 * A094084  Denominators associated with A094083.
 * https://oeis.org/A094084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of r(n), where r(1) = 1, r(2) = 1/3, r(n) =
 *            r(n-2) * ((n-2)/(n-1))^2 for n >= 3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A094084_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t num = 1, den;
    idx_t k;
    if (n & 1) {
        den = 1;
        k = 3;
    } else {
        den = 3;
        k = 4;
    }
    for (; k <= n; k += 2) {
        seq_t mn = (seq_t)(k - 2);
        seq_t md = (seq_t)(k - 1);
        seq_t mn2 = mn * mn;
        seq_t md2 = md * md;
        /* num/den is reduced and gcd(mn, md) = 1, so cancelling
           num against md^2 and den against mn^2 gives a reduced result */
        seq_t g1 = A094084_gcd(num, md2);
        seq_t g2 = A094084_gcd(den, mn2);
        num /= g1;
        md2 /= g1;
        den /= g2;
        mn2 /= g2;
        num = num * mn2;
        den = den * md2;
    }
    return den;
}
