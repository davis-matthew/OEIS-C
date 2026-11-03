/*
 * A002688  Denominators of coefficients for repeated integration.
 * https://oeis.org/A002688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((1/n!) * Sum_{k=1..n}
 *            Stirling1(n,k)/((k+1)*(k+2))). Since 1/((k+1)(k+2)) = Int_0^1
 *            t^k (1-t) dt, the g.f. is x/log(1+x)^2 - 1/log(1+x), which gives
 *            a(n) = denominator(-(n+1)*G(n+1) - (n-1)*G(n)), where G(m) =
 *            (1/m!) * Sum_{k=1..m} Stirling1(m,k)/(k+1) are the Gregory
 *            coefficie...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 420 (2026-11-03)
 * Helpers:   gcd, gcd_idx, mem, modinv, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/gcd_idx.h"
#include "../utils/modinv.h"
#include "../utils/mulmod.h"
#include "../utils/mem.h"

/* Exact computation with arbitrary-precision seq_t. */
static seq_t a2688_big(idx_t n) {
    idx_t N = n + 2, i, k, row;
    seq_t M = 1;
    for (i = 2; i <= N; i++) {
        idx_t r = (idx_t)(M % (seq_t)i);
        idx_t g = gcd_idx(i, r);
        M = M / (seq_t)g * (seq_t)i;
    }
    seq_t *w = mem_new(seq_t, N + 1);
    seq_t *c = mem_new(seq_t, N + 1);
    for (k = 0; k <= N; k++) { w[k] = 0; c[k] = 0; }
    for (k = 1; k <= n + 1; k++) w[k] = M / (seq_t)(k + 1);
    c[0] = 1;
    seq_t Pn = 0, Qn = 0, P1 = 0, Q1 = 0;
    for (row = 1; row <= n + 1; row++) {
        for (k = row; k >= 1; k--)
            c[k] = c[k - 1] + (seq_t)(row - 1) * c[k];
        c[0] = 0;
        if (row == n || row == n + 1) {
            seq_t P = 0, Q = 0;
            for (k = 1; k <= row; k++) {
                if (((row - k) & 1) == 0) P += c[k] * w[k];
                else Q += c[k] * w[k];
            }
            if (row == n) { Pn = P; Qn = Q; }
            else { P1 = P; Q1 = Q; }
        }
    }
    seq_t A = P1 + (seq_t)(n - 1) * Pn;
    seq_t B = Q1 + (seq_t)(n - 1) * Qn;
    seq_t diff = (A >= B) ? A - B : B - A;
    seq_t X = (seq_t)(n + 1) * diff;
    seq_t D = M;
    for (i = 2; i <= n + 1; i++) D *= (seq_t)i;
    seq_t g = gcd(X, D);
    mem_free(w);
    mem_free(c);
    return D / g;
}

/* Fixed-width build: only small n fit, so find each prime's exponent modulo p^e. */
static seq_t a2688_fixed(idx_t n) {
    if (n > 200) return 0; /* far beyond any fixed-width range */
    idx_t N = n + 2, k, row;
    uint64_t *c = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    uint64_t *w = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    seq_t res = 1;
    uint64_t p;
    for (p = 2; p <= N; p++) {
        bool prime = true;
        uint64_t d;
        for (d = 2; d * d <= p; d++) if (p % d == 0) { prime = false; break; }
        if (!prime) continue;
        int eM = 0;
        uint64_t t = 1;
        while (t <= N / p) { t *= p; eM++; }
        int eF = 0;
        uint64_t q;
        for (q = p; q <= n + 1; q *= p) {
            eF += (int)((n + 1) / q);
            if (q > (n + 1) / p) break;
        }
        int e = eM + eF;
        uint64_t m = 1;
        int ec = 0;
        while (ec < e && m <= (UINT64_C(1) << 62) / p) { m *= p; ec++; }
        for (k = 1; k <= n + 1; k++) {
            uint64_t u = k + 1;
            int v = 0;
            while (u % p == 0) { u /= p; v++; }
            uint64_t pw = 1;
            int j;
            for (j = 0; j < eM - v; j++) pw = mulmod(pw, p, m);
            w[k] = mulmod(pw, modinv(u, m), m);
        }
        for (k = 0; k <= N; k++) c[k] = 0;
        c[0] = 1 % m;
        uint64_t Pn = 0, Qn = 0, P1 = 0, Q1 = 0;
        for (row = 1; row <= n + 1; row++) {
            for (k = row; k >= 1; k--)
                c[k] = (c[k - 1] + mulmod((row - 1) % m, c[k], m)) % m;
            c[0] = 0;
            if (row == n || row == n + 1) {
                uint64_t P = 0, Q = 0;
                for (k = 1; k <= row; k++) {
                    uint64_t term = mulmod(c[k], w[k], m);
                    if (((row - k) & 1) == 0) P = (P + term) % m;
                    else Q = (Q + term) % m;
                }
                if (row == n) { Pn = P; Qn = Q; }
                else { P1 = P; Q1 = Q; }
            }
        }
        uint64_t A = (P1 + mulmod((n - 1) % m, Pn, m)) % m;
        uint64_t B = (Q1 + mulmod((n - 1) % m, Qn, m)) % m;
        uint64_t dd = (A + m - B) % m;
        int vd = 0;
        if (dd == 0) vd = ec;
        else while (dd % p == 0) { dd /= p; vd++; }
        int vX = vd;
        uint64_t n1 = n + 1;
        while (n1 % p == 0) { n1 /= p; vX++; }
        int j;
        for (j = vX; j < e; j++) res *= (seq_t)p;
    }
    free(c);
    free(w);
    return res;
}

static int a2688_fixed_build(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x += x;
    return x == 0;
}

seq_t a(idx_t n) {
    if (a2688_fixed_build()) return a2688_fixed(n);
    return a2688_big(n);
}
