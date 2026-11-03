/*
 * A000743  Number of compositions of n into 5 ordered relatively prime parts.
 * https://oeis.org/A000743
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * binomial(n/d - 1, 4) (Moebius transform
 *            of binomial(n-1,4))
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 570, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 8388 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binomial(m, 4) computed exactly (mod 2^w for fixed-width seq_t) */
static seq_t a743_binom4(uint64_t m) {
    uint64_t f[4];
    int i, k;
    seq_t r;
    if (m < 4) return (seq_t)0;
    for (i = 0; i < 4; i++) f[i] = m - (uint64_t)i;
    for (k = 0; k < 3; k++) {
        for (i = 0; i < 4; i++) {
            if (f[i] % 2 == 0) { f[i] /= 2; break; }
        }
    }
    for (i = 0; i < 4; i++) {
        if (f[i] % 3 == 0) { f[i] /= 3; break; }
    }
    r = (seq_t)1;
    for (i = 0; i < 4; i++) r *= (seq_t)f[i];
    return r;
}

static int a743_mu(uint64_t m) {
    int r = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            r = -r;
        }
    }
    if (m > 1) r = -r;
    return r;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            idx_t q = n / d;
            int mu;
            if (q >= 5) {
                mu = a743_mu((uint64_t)d);
                if (mu > 0) result += a743_binom4((uint64_t)(q - 1));
                else if (mu < 0) result -= a743_binom4((uint64_t)(q - 1));
            }
            if (d != q && d >= 5) {
                mu = a743_mu((uint64_t)q);
                if (mu > 0) result += a743_binom4((uint64_t)(d - 1));
                else if (mu < 0) result -= a743_binom4((uint64_t)(d - 1));
            }
        }
    }
    return result;
}
