/*
 * A054195  Binomial transform of A001371.
 * https://oeis.org/A054195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*A001371(k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-07)
 * Helpers:   mem, pow2, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/totient_u64.h"
#include "../utils/mem.h"

static uint64_t A001371_pow2_divmod(idx_t e, uint64_t M, seq_t *q) {
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

static seq_t A001371_bracelets(idx_t m) {
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
        r = A001371_pow2_divmod(e, M, &q);
        Q += q * (seq_t)ph;
        R += ph * r;
        if (e != d) {
            ph = totient_u64(e);
            r = A001371_pow2_divmod(d, M, &q);
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

static int A001371_mu(uint64_t m) {
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
        int mu = A001371_mu(d);
        if (mu > 0) pos += A001371_bracelets(e);
        else if (mu < 0) neg += A001371_bracelets(e);
        if (e != d) {
            mu = A001371_mu(e);
            if (mu > 0) pos += A001371_bracelets(d);
            else if (mu < 0) neg += A001371_bracelets(d);
        }
    }
    return pos - neg;
}

seq_t a(idx_t n) {
    seq_t *row = mem_new(seq_t, (size_t)n + 1);
    row[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t j = i; j >= 1; j--) {
            row[j] += row[j - 1];
        }
    }
    seq_t total = 0;
    for (idx_t k = 0; k <= n; k++) {
        total += row[k] * A001371_a(k);
    }
    mem_free(row);
    return total;
}
