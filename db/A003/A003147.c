/*
 * A003147  Primes p with a Fibonacci primitive root g, i.e., such that g^2 =
 *          g + 1 (mod p).
 * https://oeis.org/A003147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   search: a(n) = n-th prime p with a root g of g^2 = g + 1 (mod p)
 *            that is a primitive root mod p. Except for p = 5, this needs p
 *            == +-1 (mod 10); then g = (1 +- sqrt(5))/2 mod p, found by
 *            Tonelli-Shanks
 * Range:     u8 n <= 17, u16 n <= 1749, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9867 (2026-11-01)
 * Helpers:   powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod_u64.h"

static bool a003147_isprim(uint64_t g, uint64_t p, const uint64_t *qs, int nq) {
    int i;
    if (g == 0) return false;
    for (i = 0; i < nq; i++)
        if (powmod_u64(g, (p - 1) / qs[i], p) == 1) return false;
    return true;
}

static uint64_t a003147_sqrt(uint64_t a, uint64_t p) {
    uint64_t Q = p - 1, S = 0, z = 2, M, c, t, R;
    while ((Q & 1) == 0) { Q >>= 1; S++; }
    while (powmod_u64(z, (p - 1) / 2, p) != p - 1) z++;
    M = S;
    c = powmod_u64(z, Q, p);
    t = powmod_u64(a, Q, p);
    R = powmod_u64(a, (Q + 1) / 2, p);
    while (t != 1) {
        uint64_t i = 0, tt = t, b, j;
        while (tt != 1) { tt = (uint64_t)((unsigned __int128)tt * tt % p); i++; }
        b = c;
        for (j = 0; j + i + 1 < M; j++) b = (uint64_t)((unsigned __int128)b * b % p);
        M = i;
        c = (uint64_t)((unsigned __int128)b * b % p);
        t = (uint64_t)((unsigned __int128)t * c % p);
        R = (uint64_t)((unsigned __int128)R * b % p);
    }
    return R;
}

static bool a003147_ok(uint64_t p) {
    uint64_t r, s, inv2, g1, g2, m, d;
    uint64_t qs[64];
    int nq = 0;
    if (p == 5) return true;
    if (p < 7) return false;
    r = p % 10;
    if (r != 1 && r != 9) return false;
    s = a003147_sqrt(5 % p, p);
    inv2 = (p + 1) / 2;
    g1 = (uint64_t)((unsigned __int128)((1 + s) % p) * inv2 % p);
    g2 = (uint64_t)((unsigned __int128)((1 + p - s) % p) * inv2 % p);
    m = p - 1;
    for (d = 2; d * d <= m; d++) {
        if (m % d == 0) {
            qs[nq++] = d;
            while (m % d == 0) m /= d;
        }
    }
    if (m > 1) qs[nq++] = m;
    return a003147_isprim(g1, p, qs, nq) || a003147_isprim(g2, p, qs, nq);
}

static uint64_t *a003147_terms = NULL;
static idx_t a003147_cnt = 0;
static uint64_t a003147_limit = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (a003147_cnt < n) {
        uint64_t newlim = a003147_limit ? a003147_limit * 2 : 65536;
        unsigned char *comp = (unsigned char *)calloc((size_t)newlim + 1, 1);
        uint64_t i, j;
        idx_t cap = 1024, cnt = 0;
        uint64_t *terms = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
        for (i = 2; i <= newlim; i++) {
            if (comp[i]) continue;
            for (j = i * i; j <= newlim; j += i) comp[j] = 1;
            if (a003147_ok(i)) {
                if (cnt == cap) {
                    cap *= 2;
                    terms = (uint64_t *)realloc(terms, (size_t)cap * sizeof(uint64_t));
                }
                terms[cnt++] = i;
            }
        }
        free(comp);
        free(a003147_terms);
        a003147_terms = terms;
        a003147_cnt = cnt;
        a003147_limit = newlim;
    }
    return (seq_t)a003147_terms[n - 1];
}
