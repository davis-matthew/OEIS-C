/*
 * A069551  Numbers k such that the squarefree part of k equals A062799(k).
 * https://oeis.org/A069551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   k = s*m^2 with s = A007913(k); for fixed m, choose the set G of
 *            primes of m with odd exponent and r new primes; then A062799(k)
 *            = Sum_{p|k} e_p*tau(k)/(e_p+1) depends only on (m,G,r), and we
 *            require A062799(k) = prod(G) * h with h squarefree, coprime to
 *            m, omega(h) = r. Search all k <= B, enlargi...
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 902 (2026-11-07)
 * Helpers:   is_prime_u64, is_square_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/is_square_u64.h"

static uint64_t *A069551_terms = 0;
static size_t A069551_cnt = 0;
static size_t A069551_cap = 0;
static uint64_t A069551_B = 0;

static uint64_t A069551_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

/* h squarefree, coprime to m, with exactly r distinct prime factors */
static int A069551_check(uint64_t h, uint64_t m, int r) {
    uint64_t R, d;
    int c = 0;
    if (r == 0) return h == 1;
    if (h <= 1) return 0;
    if (A069551_gcd(h, m) != 1) return 0;
    R = h;
    d = 2;
    while ((unsigned __int128)d * d * d <= R) {
        if (R % d == 0) {
            R /= d;
            if (R % d == 0) return 0;
            c++;
            if (c > r) return 0;
        }
        d = (d == 2) ? 3 : d + 2;
    }
    if (R > 1) {
        if (is_prime_u64(R)) c += 1;
        else if (is_square_u64(R)) return 0;
        else c += 2;
    }
    return c == r;
}

static void A069551_push(uint64_t k) {
    if (A069551_cnt == A069551_cap) {
        size_t nc = A069551_cap ? A069551_cap * 2 : 1024;
        uint64_t *np = (uint64_t *)realloc(A069551_terms, nc * sizeof(uint64_t));
        if (!np) return;
        A069551_terms = np;
        A069551_cap = nc;
    }
    A069551_terms[A069551_cnt++] = k;
}

static int A069551_cmp(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static int A069551_gen(uint64_t B) {
    uint64_t M = 1, m;
    uint32_t *spf;
    while ((M + 1) * (M + 1) <= B) M++;
    spf = (uint32_t *)calloc((size_t)M + 1, sizeof(uint32_t));
    if (!spf) return 0;
    for (m = 2; m <= M; m++) {
        if (spf[m] == 0) {
            uint64_t j;
            for (j = m; j <= M; j += m) if (spf[j] == 0) spf[j] = (uint32_t)m;
        }
    }
    A069551_cnt = 0;
    for (m = 1; m <= M; m++) {
        uint64_t pr[16];
        int vv[16];
        int w = 0, i;
        uint64_t t = m, msq = m * m, lim;
        unsigned mask;
        lim = B / msq;
        if (lim < 2) break;
        while (t > 1) {
            uint32_t p = spf[t];
            int v = 0;
            while (t % p == 0) { t /= p; v++; }
            pr[w] = p; vv[w] = v; w++;
        }
        for (mask = 0; mask < (1u << w); mask++) {
            unsigned __int128 g = 1, T0 = 1, A = 0;
            int r;
            for (i = 0; i < w; i++) {
                int e = 2 * vv[i] + ((mask >> i) & 1);
                T0 *= (unsigned __int128)(e + 1);
                if ((mask >> i) & 1) g *= pr[i];
            }
            if (g > lim) continue;
            for (i = 0; i < w; i++) {
                int e = 2 * vv[i] + ((mask >> i) & 1);
                A += (unsigned __int128)e * (T0 / (unsigned __int128)(e + 1));
            }
            for (r = 0; r < 63; r++) {
                unsigned __int128 S = A << r;
                if (r > 0) S += (unsigned __int128)r * (T0 << (r - 1));
                if (S > lim) break;
                if (S % g == 0 && A069551_check((uint64_t)(S / g), m, r))
                    A069551_push((uint64_t)S * msq);
            }
        }
    }
    free(spf);
    qsort(A069551_terms, A069551_cnt, sizeof(uint64_t), A069551_cmp);
    A069551_B = B;
    return 1;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (A069551_B == 0) A069551_gen((uint64_t)1 << 20);
    while (A069551_cnt < (size_t)n) {
        uint64_t nb;
        if (A069551_B >= ((uint64_t)1 << 60)) break;
        nb = A069551_B * 4;
        if (!A069551_gen(nb)) break;
    }
    if ((size_t)n > A069551_cnt) return 0;
    return (seq_t)A069551_terms[n - 1];
}
