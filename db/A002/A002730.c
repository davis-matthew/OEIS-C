/*
 * A002730  Number of equivalence classes of binary sequences of primitive
 *          period n.
 * https://oeis.org/A002730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{d|n} mu(n/d)*N(d), where N(d) = A002729(d) is the
 *            number of orbits of binary sequences of period d under the
 *            affine maps x -> u*x+b (gcd(u,d)=1). N(d) is computed by
 *            Burnside: N(d) = (1/(d*phi(d))) * Sum_{u,b} 2^{cycles(x->u*x+b
 *            mod d)}, with cycles(f) = (1/L) Sum_{j|L} phi(L/j)*Fix(f...
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 41, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   x2730_gcd, x2730_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/x2730_gcd.h"
#include "../utils/x2730_mu.h"

static uint32_t *x2730_spf = NULL;
static idx_t x2730_spf_n = 0;

static void x2730_sieve(idx_t n) {
    idx_t i, j;
    if (n <= x2730_spf_n && x2730_spf) return;
    free(x2730_spf);
    x2730_spf = (uint32_t *)malloc((size_t)(n + 1) * sizeof(uint32_t));
    for (i = 0; i <= n; i++) x2730_spf[i] = 0;
    for (i = 2; i <= n; i++) {
        if (x2730_spf[i] == 0) {
            for (j = i; j <= n; j += i)
                if (x2730_spf[j] == 0) x2730_spf[j] = (uint32_t)i;
        }
    }
    x2730_spf_n = n;
}

/* add factorization of v into (p,e) list of length *np */
static void x2730_addfac(uint64_t v, uint64_t *p, int *e, int *np) {
    while (v > 1) {
        uint64_t q = x2730_spf[v];
        int c = 0, i;
        while (v % q == 0) { v /= q; c++; }
        for (i = 0; i < *np; i++) if (p[i] == q) break;
        if (i == *np) { p[i] = q; e[i] = 0; (*np)++; }
        e[i] += c;
    }
}

static seq_t x2730_N(idx_t n) {
    uint64_t *cnt, *powv, *sv;
    uint64_t a, phin = 0, M;
    seq_t q;
    uint64_t r;
    idx_t c;
    if (n == 1) return (seq_t)2;
    x2730_sieve(n);
    cnt = (uint64_t *)calloc((size_t)(n + 1), sizeof(uint64_t));
    powv = (uint64_t *)malloc((size_t)(n + 2) * sizeof(uint64_t));
    sv = (uint64_t *)malloc((size_t)(n + 2) * sizeof(uint64_t));
    for (a = 1; a < n; a++) {
        uint64_t k, Sk, g, b, kp[64];
        int ke[64], knp = 0;
        if (x2730_gcd(a, n) != 1) continue;
        phin++;
        k = 0; powv[0] = 1; sv[0] = 0;
        do {
            sv[k + 1] = (sv[k] + powv[k]) % n;
            powv[k + 1] = powv[k] * a % n;
            k++;
        } while (powv[k] != 1);
        Sk = sv[k];
        g = x2730_gcd(a - 1, n);
        x2730_addfac(k, kp, ke, &knp);
        for (b = 0; b < g; b++) {
            uint64_t t = b * Sk % n;
            uint64_t m = n / x2730_gcd(t, n);
            uint64_t p[64], L = k * m, d = 1, sum = 0;
            int E[64], ex[64], np = knp, i;
            for (i = 0; i < knp; i++) { p[i] = kp[i]; E[i] = ke[i]; }
            x2730_addfac(m, p, E, &np);
            for (i = 0; i < np; i++) ex[i] = 0;
            for (;;) {
                uint64_t ph = 1, rr = d % k, qq = d / k, A, B, gA;
                int j;
                for (j = 0; j < np; i = 0, j++) {
                    int ee = E[j] - ex[j], s;
                    if (ee > 0) {
                        ph *= p[j] - 1;
                        for (s = 1; s < ee; s++) ph *= p[j];
                    }
                }
                A = powv[rr];
                B = b * (((qq % n) * Sk + sv[rr]) % n) % n;
                gA = x2730_gcd((A + n - 1) % n, n);
                if (B % gA == 0) sum += ph * gA;
                i = 0;
                while (i < np) {
                    if (ex[i] < E[i]) { ex[i]++; d *= p[i]; break; }
                    while (ex[i] > 0) { d /= p[i]; ex[i]--; }
                    i++;
                }
                if (i == np) break;
            }
            cnt[sum / L] += n / g;
        }
    }
    M = (uint64_t)n * phin;
    q = 0; r = 0;
    for (c = n; ; c--) {
        r = 2 * r + cnt[c];
        q = q * 2 + (seq_t)(r / M);
        r %= M;
        if (c == 0) break;
    }
    free(cnt); free(powv); free(sv);
    return q;
}

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    idx_t d;
    for (d = 1; d <= n; d++) {
        if (n % d == 0) {
            int mu = x2730_mu((uint64_t)(n / d));
            if (mu == 0) continue;
            if (mu > 0) pos += x2730_N(d);
            else neg += x2730_N(d);
        }
    }
    return pos - neg;
}
