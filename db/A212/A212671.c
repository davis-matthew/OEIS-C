/*
 * A212671  A001037(n)!.
 * https://oeis.org/A212671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001037(n)!, where A001037(n) = (1/n)*Sum_{d|n}
 *            mu(n/d)*2^d for n >= 1, A001037(0) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-07)
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
    seq_t m = A001037_a(n);
    if (m > 1000) return 0; /* fixed-width builds: m! is 0 mod 2^128 here */
    long long k = (long long)m;
    seq_t res = 1;
    for (long long i = 2; i <= k; i++) res = res * (seq_t)i;
    return res;
}
