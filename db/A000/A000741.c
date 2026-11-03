/*
 * A000741  Number of compositions of n into 3 ordered relatively prime parts.
 * https://oeis.org/A000741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * binomial(d-1, 2) (Moebius transform
 *            of A000217(n-2))
 * Range:     u8 n <= 24, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8501 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binomial(d-1, 2) = (d-1)(d-2)/2 */
static seq_t a741_c2(idx_t d) {
    idx_t x, y;
    if (d < 3) return 0;
    x = d - 1;
    y = d - 2;
    if (x % 2 == 0) x /= 2; else y /= 2;
    return (seq_t)x * (seq_t)y;
}

static int a741_mu(uint64_t m) {
    int r = 1;
    uint64_t p;
    if (m == 1) return 1;
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

static void a741_add(idx_t n, idx_t d, seq_t *pos, seq_t *neg) {
    int mu = a741_mu(n / d);
    if (mu == 0) return;
    if (mu > 0) *pos += a741_c2(d);
    else *neg += a741_c2(d);
}

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    idx_t d;
    if (n < 3) return 0;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            idx_t q = n / d;
            a741_add(n, d, &pos, &neg);
            if (q != d) a741_add(n, q, &pos, &neg);
        }
    }
    return pos - neg;
}
