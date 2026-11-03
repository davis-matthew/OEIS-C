/*
 * A000742  Number of compositions of n into 4 ordered relatively prime parts.
 * https://oeis.org/A000742
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * C(n/d - 1, 3) (Moebius transform of
 *            C(n-1,3))
 * Range:     u8 n <= 13, u16 n <= 76, u32 n <= 2956, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9378 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* C(k-1, 3) computed exactly, dividing out 2 and 3 before multiplying */
static seq_t a742_c3(idx_t k) {
    idx_t x, y, z;
    if (k < 4) return 0;
    x = k - 1; y = k - 2; z = k - 3;
    if (x % 2 == 0) x /= 2; else y /= 2;
    if (x % 3 == 0) x /= 3; else if (y % 3 == 0) y /= 3; else z /= 3;
    return (seq_t)x * (seq_t)y * (seq_t)z;
}

static int a742_mu(uint64_t m) {
    int cnt = 0;
    uint64_t p;
    if (m == 1) return 1;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            cnt++;
        }
    }
    if (m > 1) cnt++;
    return (cnt & 1) ? -1 : 1;
}

static void a742_add(seq_t *pos, seq_t *neg, idx_t n, idx_t d) {
    int mu = a742_mu(d);
    if (mu > 0) *pos += a742_c3(n / d);
    else if (mu < 0) *neg += a742_c3(n / d);
}

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            a742_add(&pos, &neg, n, d);
            if (n / d != d) a742_add(&pos, &neg, n, n / d);
        }
    }
    return pos - neg;
}
