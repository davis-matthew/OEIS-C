/*
 * A054158  Inverse Moebius transform of A001371 (starting at term 0).
 * https://oeis.org/A054158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n+1} A001371(d-1), where A001371(m) = Sum_{d|m}
 *            mu(d)*A000029(m/d), A001371(0) = 1
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-07)
 * Helpers:   pow2, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/totient_u64.h"

static uint64_t l_pow2_divmod(idx_t e, uint64_t M, seq_t *q) {
    seq_t qq = 0;
    uint64_t r = 1 % M;
    if (M == 1) {
        *q = pow2(e);
        return 0;
    }
    for (idx_t i = 0; i < e; i++) {
        qq += qq;
        r <<= 1;
        if (r >= M) {
            r -= M;
            qq += 1;
        }
    }
    *q = qq;
    return r;
}

static seq_t l_bracelets(idx_t m) {
    if (m == 1) return 2;
    if (m == 2) return 3;
    uint64_t M = 2 * (uint64_t)m;
    seq_t Q = 0;
    uint64_t R = 0;
    for (idx_t d = 1; d * d <= m; d++) {
        if (m % d != 0) continue;
        idx_t e = m / d;
        seq_t q;
        uint64_t r, ph;
        ph = totient_u64(d);
        r = l_pow2_divmod(e, M, &q);
        Q += q * (seq_t)ph;
        R += ph * r;
        if (e != d) {
            ph = totient_u64(e);
            r = l_pow2_divmod(d, M, &q);
            Q += q * (seq_t)ph;
            R += ph * r;
        }
    }
    Q += (seq_t)(R / M);
    if (m % 2 == 1)
        Q += pow2((m - 1) / 2);
    else
        Q += (seq_t)3 * pow2(m / 2 - 2);
    return Q;
}

static int l_mu(uint64_t m) {
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

static seq_t A001371_a(idx_t n) {
    if (n == 0) return 1;
    seq_t pos = 0, neg = 0;
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d != 0) continue;
        idx_t e = n / d;
        int mu = l_mu(d);
        if (mu > 0) pos += l_bracelets(e);
        else if (mu < 0) neg += l_bracelets(e);
        if (e != d) {
            mu = l_mu(e);
            if (mu > 0) pos += l_bracelets(d);
            else if (mu < 0) neg += l_bracelets(d);
        }
    }
    return pos - neg;
}

seq_t a(idx_t n) {
    idx_t N = n + 1;
    seq_t s = 0;
    for (idx_t d = 1; d * d <= N; d++) {
        if (N % d != 0) continue;
        idx_t e = N / d;
        s += A001371_a(d - 1);
        if (e != d) s += A001371_a(e - 1);
    }
    return s;
}
