/*
 * A001693  Number of degree-n irreducible polynomials over GF(7); dimensions
 *          of free Lie algebras.
 * https://oeis.org/A001693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/n)*Sum_{d|n} mu(d)*7^(n/d), for n>0; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 1087 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void a1693_add(seq_t *Q, uint64_t *R, seq_t q, uint64_t r, idx_t n) {
    *Q += q;
    *R += r;
    if (*R >= n) {
        *R -= n;
        *Q += 1;
    }
}

static int a1693_mu(uint64_t m) {
    int r = 1;
    uint64_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            r = -r;
        }
    }
    if (m > 1) r = -r;
    return r;
}

/* 7^e = q*n + r with 0 <= r < n; q is computed modulo the width of seq_t */
static void a1693_pow7_qr(idx_t e, idx_t n, seq_t *q, uint64_t *r) {
    seq_t Q = (seq_t)(1 / n);
    uint64_t R = 1 % n;
    idx_t i;
    for (i = 0; i < e; i++) {
        R *= 7;
        Q = Q * 7 + (seq_t)(R / n);
        R %= n;
    }
    *q = Q;
    *r = R;
}

seq_t a(idx_t n) {
    seq_t Qp = 0, Qn = 0;
    uint64_t Rp = 0, Rn = 0;
    idx_t d;
    if (n == 0) return 1;
    for (d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            idx_t ds[2];
            int cnt = 1, k;
            ds[0] = d;
            if (d != n / d) { ds[1] = n / d; cnt = 2; }
            for (k = 0; k < cnt; k++) {
                int mu = a1693_mu((uint64_t)ds[k]);
                seq_t q;
                uint64_t r;
                if (mu == 0) continue;
                a1693_pow7_qr(n / ds[k], n, &q, &r);
                if (mu > 0) a1693_add(&Qp, &Rp, q, r, n);
                else a1693_add(&Qn, &Rn, q, r, n);
            }
        }
    }
    /* the total is divisible by n, so Rp == Rn */
    return Qp - Qn;
}
