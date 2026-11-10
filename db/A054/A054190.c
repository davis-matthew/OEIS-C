/*
 * A054190  Binomial transform of A001037.
 * https://oeis.org/A054190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*A001037(k), A001037(0)=1,
 *            A001037(k) = (1/k)*Sum_{d|k} mu(k/d)*2^d
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A001037_mu(uint64_t m) {
    int r = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            r = -r;
        }
    }
    if (m > 1) r = -r;
    return r;
}

/* floor(2^d / n) by binary long division; *rem receives 2^d mod n */
static seq_t A001037_qdiv(idx_t d, uint64_t n, uint64_t *rem) {
    seq_t q = 0;
    uint64_t r = 0;
    for (idx_t i = 0; i <= d; i++) {
        r = 2 * r + (i == 0 ? 1 : 0);
        q = q + q;
        if (r >= n) {
            r -= n;
            q = q + 1;
        }
    }
    *rem = r;
    return q;
}

static seq_t A001037_a(idx_t n) {
    if (n == 0) return 1;
    seq_t pos = 0, neg = 0;
    long long R = 0;
    for (idx_t d = 1; d <= n; d++) {
        if (n % d != 0) continue;
        int mu = A001037_mu(n / d);
        if (mu == 0) continue;
        uint64_t r;
        seq_t q = A001037_qdiv(d, n, &r);
        if (mu > 0) {
            pos = pos + q;
            R += (long long)r;
        } else {
            neg = neg + q;
            R -= (long long)r;
        }
    }
    seq_t res = pos - neg;
    long long c = R / (long long)n;
    if (c >= 0) res = res + (seq_t)(unsigned long long)c;
    else res = res - (seq_t)(unsigned long long)(-c);
    return res;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    seq_t c = 1; /* binomial(n,k) */
    for (idx_t k = 0; k <= n; k++) {
        if (k > 0) {
            c = c * (seq_t)(n - k + 1) / (seq_t)k;
        }
        total = total + c * A001037_a(k);
    }
    return total;
}
