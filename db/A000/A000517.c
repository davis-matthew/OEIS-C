/*
 * A000517  Number of permutations of length n with exactly three valleys.
 * https://oeis.org/A000517
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.:
 *            16x^7(17-184x+636x^2-720x^3)/((1-2x)^4*(1-4x)^3*(1-6x)^2*(1-8x)),
 *            computed by the linear recurrence D(x)*A(x) = N(x), where D is
 *            the expanded denominator (built at runtime), and a(7)=272 is
 *            c(0)
 * Range:     u8 none, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

seq_t a(idx_t n) {
    long long d[11];
    int deg = 0, f, k;
    const long long roots[10] = {2, 2, 2, 2, 4, 4, 4, 6, 6, 8};
    const long long num[4] = {272, -2944, 10176, -11520};
    seq_t win[11];
    idx_t i, m;

    if (n < 7) return 0;

    for (k = 0; k < 11; k++) d[k] = 0;
    d[0] = 1;
    for (f = 0; f < 10; f++) {
        long long r = roots[f];
        for (k = deg + 1; k >= 1; k--) d[k] -= r * d[k - 1];
        deg++;
    }

    for (k = 0; k < 11; k++) win[k] = 0;
    m = n - 7;
    for (i = 0; i <= m; i++) {
        seq_t pos = 0, neg = 0;
        if (i < 4) {
            if (num[i] >= 0) pos += (seq_t)(unsigned long long)num[i];
            else neg += (seq_t)(unsigned long long)(-num[i]);
        }
        for (k = 1; k <= 10 && (idx_t)k <= i; k++) {
            seq_t v = win[(i - (idx_t)k) % 11];
            if (d[k] < 0) pos += (seq_t)(unsigned long long)(-d[k]) * v;
            else if (d[k] > 0) neg += (seq_t)(unsigned long long)d[k] * v;
        }
        win[i % 11] = pos - neg;
    }
    return win[m % 11];
}
