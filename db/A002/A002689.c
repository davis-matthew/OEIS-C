/*
 * A002689  Denominators of coefficients for repeated integration.
 * https://oeis.org/A002689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A002207(n)/(n+1), where A002207(n) = denominator(G(n+1)),
 *            the Gregory coefficients G(m) = (1/m!) * Sum_{k=1..m}
 *            Stirling1(m,k)/(k+1). With L = lcm(1..m+1), N = Sum_{k=1..m}
 *            Stirling1(m,k)*L/(k+1) and D = m!*L, the reduced denominator is
 *            D/gcd(N,D)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 181 (2026-11-03)
 * Helpers:   is_prime_u64, mem, mulmod, valuation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/valuation.h"
#include "../utils/mem.h"

/* largest f with p^f <= x */
static uint64_t a2689_flog(uint64_t p, uint64_t x) {
    uint64_t f = 0, q = 1;
    while (q <= x / p) { q *= p; f++; }
    return f;
}

/* (L / k1) mod M, L = lcm(1..m+1) */
static uint64_t a2689_lk(uint64_t k1, uint64_t m, uint64_t M) {
    uint64_t r = 1 % M;
    for (uint64_t q = 2; q <= m + 1; q++) {
        if (!is_prime_u64(q)) continue;
        uint64_t ex = a2689_flog(q, m + 1) - valuation(k1, q);
        for (uint64_t i = 0; i < ex; i++) r = mulmod(r, q % M, M);
    }
    return r;
}

static uint64_t a2689_vfact(uint64_t m, uint64_t p) {
    uint64_t s = 0;
    while (m) { m /= p; s += m; }
    return s;
}

/* p-adic computation with native moduli; returns 0 if a modulus would not fit */
static int a2689_small(uint64_t m, seq_t *out) {
    uint64_t c[70];
    seq_t res = 1;
    if (m > 63) return 0;
    for (uint64_t p = 2; p <= m + 1; p++) {
        if (!is_prime_u64(p)) continue;
        uint64_t e = a2689_vfact(m, p) + a2689_flog(p, m + 1);
        uint64_t M = 1;
        for (uint64_t i = 0; i < e; i++) {
            if (M > ((uint64_t)1 << 62) / p) return 0;
            M *= p;
        }
        for (uint64_t k = 0; k <= m; k++) c[k] = 0;
        c[1] = 1 % M;
        for (uint64_t j = 1; j < m; j++)
            for (uint64_t k = j + 1; k >= 1; k--)
                c[k] = (c[k - 1] + mulmod(j % M, c[k], M)) % M;
        uint64_t N = 0;
        for (uint64_t k = 1; k <= m; k++) {
            uint64_t t = mulmod(c[k], a2689_lk(k + 1, m, M), M);
            if ((m - k) & 1) N = (N + M - t) % M;
            else N = (N + t) % M;
        }
        uint64_t v = 0;
        if (N == 0) v = e;
        else while (v < e && N % p == 0) { N /= p; v++; }
        uint64_t vm = valuation(m, p);
        uint64_t ex = e - v;
        ex = ex >= vm ? ex - vm : 0;
        for (uint64_t i = 0; i < ex; i++) res *= (seq_t)p;
    }
    *out = res;
    return 1;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n + 1;
    seq_t r;
    if (a2689_small(m, &r)) return r;

    /* exact path (arbitrary precision) */
    seq_t *c = mem_new(seq_t, (size_t)(m + 1));
    for (uint64_t k = 0; k <= m; k++) c[k] = 0;
    c[1] = 1;
    for (uint64_t j = 1; j < m; j++)
        for (uint64_t k = j + 1; k >= 1; k--)
            c[k] = c[k - 1] + c[k] * (seq_t)j;
    seq_t L = 1;
    for (uint64_t p = 2; p <= m + 1; p++) {
        if (!is_prime_u64(p)) continue;
        uint64_t f = a2689_flog(p, m + 1);
        for (uint64_t i = 0; i < f; i++) L *= (seq_t)p;
    }
    seq_t pos = 0, neg = 0;
    for (uint64_t k = 1; k <= m; k++) {
        seq_t t = c[k] * (L / (seq_t)(k + 1));
        if ((m - k) & 1) neg += t;
        else pos += t;
    }
    mem_free(c);
    seq_t N = pos >= neg ? pos - neg : neg - pos;
    seq_t D = L;
    for (uint64_t i = 2; i <= m; i++) D *= (seq_t)i;
    seq_t x = N, y = D;
    while (y != 0) { seq_t t = x % y; x = y; y = t; }
    if (x == 0) return 0;
    return D / x / (seq_t)m;
}
