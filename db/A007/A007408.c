/*
 * A007408  Wolstenholme numbers: numerator of Sum_{k=1..n} 1/k^3.
 * https://oeis.org/A007408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of Sum_{k=1..n} 1/k^3, accumulated as reduced
 *            fractions: num/den + 1/k^3 with g = gcd(den, k^3) gives
 *            (num*(k^3/g) + den/g) / (den/g*k^3), then reduce
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 193 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t num = 0, den = 1;
    idx_t k;

    for (k = 1; k <= n; k++) {
        seq_t ks = (seq_t)k;
        seq_t k3 = ks * ks * ks;
        seq_t g = gcd(den, k3);
        seq_t dg, kg, g2;
        if (g == 0) g = 1;
        dg = den / g;
        kg = k3 / g;
        num = num * kg + dg;
        den = dg * k3;
        g2 = gcd(num, den);
        if (g2 != 0 && g2 != 1) {
            num /= g2;
            den /= g2;
        }
    }

    return num;
}
