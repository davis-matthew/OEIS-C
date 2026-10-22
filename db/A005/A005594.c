/*
 * A005594  States of a dynamic storage system.
 * https://oeis.org/A005594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} phi(d) * (A005248(n/d) - 1) for n >= 1,
 *            a(0) = 1, where A005248(m) = Lucas(2m) satisfies s(m) = 3*s(m-1)
 *            - s(m-2), s(0) = 2, s(1) = 3. (CIK transform of 2,1,1,1,...: 1 -
 *            B(x) = (1 - 3x + x^2)/(1 - x).) To avoid overflowing
 *            intermediates, each s(k) is kept as Q_k*n + R_k w...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t totient_u64(uint64_t m) {
    uint64_t r = m, t = m, p;
    for (p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            r -= r / p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) r -= r / t;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    long long nn = (long long)n;

    /* s(0) = 2, s(1) = 3 represented as Q*n + R */
    seq_t Qp = (seq_t)(2 / n);
    long long Rp = (long long)(2 % n);
    seq_t Qc = (seq_t)(3 / n);
    long long Rc = (long long)(3 % n);

    seq_t S = 0;
    long long rem = 0;

    for (idx_t k = 1; k <= n; k++) {
        if (n % k == 0) {
            uint64_t ph = totient_u64(n / k);
            S += (seq_t)ph * Qc;
            rem += (long long)ph * (Rc - 1);
        }
        if (k == n) break;
        /* advance: s(k+1) = 3*s(k) - s(k-1) */
        long long t = 3 * Rc - Rp;
        long long c = 0;
        while (t < 0) { t += nn; c--; }
        while (t >= nn) { t -= nn; c++; }
        seq_t Qn = Qc * 3;
        Qn -= Qp;
        if (c < 0) Qn -= (seq_t)(-c);
        else Qn += (seq_t)c;
        Qp = Qc; Rp = Rc;
        Qc = Qn; Rc = t;
    }

    long long q = rem / nn; /* exact: total sum is divisible by n */
    if (q < 0) S -= (seq_t)(-q);
    else S += (seq_t)q;
    return S;
}
