/*
 * A005782  Number of n-gons in cubic curve.
 * https://oeis.org/A005782
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice
 * Formula:   n*a(n) = Sum_{d|n} mu(n/d) * (2^d - (-1)^d)^2, i.e. the Moebius
 *            inversion of f(d) = ((-2)^d - 1)^2 = 4^d - 2*(-2)^d + 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 884 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor(b^e / n) in seq_t (wrapping is harmless) and b^e mod n */
static void qr_pow(idx_t n, long long b, idx_t e, seq_t *q, long long *r)
{
    seq_t Q = 0;
    long long R = 1, nn = (long long)n;
    idx_t i;
    for (i = 0; i < e; i++) {
        R *= b;
        Q = Q * (seq_t)b + (seq_t)(R / nn);
        R %= nn;
    }
    *q = Q;
    *r = R;
}

seq_t a(idx_t n)
{
    uint64_t primes[64];
    int np = 0, k;
    uint64_t m = n, p;
    unsigned long long mask;
    seq_t Qpos = 0, Qneg = 0, res;
    long long R = 0, nn = (long long)n, t;

    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            primes[np++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) primes[np++] = m;

    for (mask = 0; mask < (1ULL << np); mask++) {
        uint64_t e = 1;
        int bits = 0;
        idx_t d;
        int s, s2;
        seq_t q;
        long long r;
        for (k = 0; k < np; k++)
            if (mask & (1ULL << k)) { e *= primes[k]; bits++; }
        d = n / e;
        s = (bits & 1) ? -1 : 1;

        /* s * 4^d */
        qr_pow(n, 4, d, &q, &r);
        if (s > 0) Qpos += q; else Qneg += q;
        R += s * r;

        /* s * (-1)^(d+1) * 2^(d+1) */
        s2 = (d & 1) ? s : -s;
        qr_pow(n, 2, d + 1, &q, &r);
        if (s2 > 0) Qpos += q; else Qneg += q;
        R += s2 * r;

        /* s * 1 */
        R += s * (1 % nn);
    }

    res = Qpos - Qneg;
    t = R / nn;
    if (t >= 0) res += (seq_t)t;
    else res -= (seq_t)(-t);
    return res;
}
