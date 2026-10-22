/*
 * A007235  Structure constants for certain representations of S_n.
 * https://oeis.org/A007235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n, 2*n+2) where b(k, m) = (2 * m! / (k^2 * (m-k)^2)) *
 *            ((m / (m+1)) * (1 + (-1)^(k+1) / binomial(m, k)) + ((k+1) /
 *            binomial(m, k)) * Sum_{j=0..k-1} (j+1) / (binomial(k - 1, j) *
 *            binomial(k, j))) Cleared of denominators (m = 2k+2, k = n): a(n)
 *            = 2*( m*(k-1)!*(m! + (-1)^(k+1)*k!*(k+2)!) +...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   seq_big_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_big_check.h"

static uint64_t powm(uint64_t b, uint64_t e, uint64_t p) {
    uint64_t r = 1;
    b %= p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

static uint64_t a_mod(idx_t k, uint64_t p) {
    idx_t m = 2 * k + 2, i, j;
    uint64_t *f = (uint64_t *)malloc((size_t)(m + 1) * sizeof(uint64_t));
    uint64_t W = 0, P, inner, N, Dn, fk1, fk2, fm;
    f[0] = 1;
    for (i = 1; i <= m; i++) f[i] = f[i - 1] * (i % p) % p;
    fk1 = f[k - 1];
    fk2 = f[k + 2];
    fm = f[m];
    for (j = 0; j < k; j++) {
        uint64_t t = ((j + 1) % p) * f[j] % p;
        t = t * f[j] % p;
        t = t * f[k - 1 - j] % p;
        t = t * f[k - j] % p;
        W = (W + t) % p;
    }
    P = f[k] * fk2 % p;
    inner = (k % 2 == 1) ? (fm + P) % p : (fm + p - P) % p;
    N = (m % p) * fk1 % p * inner % p;
    N = (N + ((m + 1) % p) * ((k + 1) % p) % p * fk2 % p * W) % p;
    N = 2 * N % p;
    Dn = ((m + 1) % p) * fk1 % p;
    Dn = Dn * (k % p) % p * (k % p) % p;
    Dn = Dn * ((k + 2) % p) % p * ((k + 2) % p) % p;
    free(f);
    return N * powm(Dn, p - 2, p) % p;
}

#define NPR 6
static uint64_t primes[NPR];
static int primes_ready = 0;

static void init_primes(void) {
    uint64_t c = 2147483647ULL;
    int cnt = 0;
    while (cnt < NPR) {
        uint64_t d;
        int ok = 1;
        for (d = 3; d * d <= c; d += 2)
            if (c % d == 0) { ok = 0; break; }
        if (ok) primes[cnt++] = c;
        c -= 2;
    }
    primes_ready = 1;
}

seq_t a(idx_t n) {
    idx_t k = n, m = 2 * n + 2, i, j;

    if (!seq_big_check()) {
        uint64_t r[NPR], d[NPR];
        unsigned __int128 v;
        seq_t res = 0;
        int s, x, y;
        if (!primes_ready) init_primes();
        for (x = 0; x < NPR; x++) r[x] = a_mod(k, primes[x]);
        for (x = 0; x < NPR; x++) {
            uint64_t p = primes[x], t = r[x];
            for (y = 0; y < x; y++) {
                t = (t + p - d[y] % p) % p;
                t = t * powm(primes[y] % p, p - 2, p) % p;
            }
            d[x] = t;
        }
        v = d[NPR - 1];
        for (x = NPR - 2; x >= 0; x--) v = v * primes[x] + d[x];
        for (s = 120; s >= 0; s -= 8)
            res = (seq_t)((seq_t)(res << 8) | (seq_t)(unsigned)((v >> s) & 255));
        return res;
    }

    {
        seq_t f = 1, fk1 = 1, fk = 1, fk2 = 1, fm, t, W = 0, P, inner, N, Dn;
        for (i = 1; i <= m; i++) {
            f *= i;
            if (i == k - 1) fk1 = f;
            if (i == k) fk = f;
            if (i == k + 2) fk2 = f;
        }
        fm = f;
        t = fk1 * fk;
        for (j = 0; j < k; j++) {
            W += t * (j + 1);
            if (j + 1 < k) {
                t *= (j + 1) * (j + 1);
                t /= (k - 1 - j) * (k - j);
            }
        }
        P = fk * fk2;
        inner = (k % 2 == 1) ? fm + P : fm - P;
        N = (seq_t)m * fk1 * inner + (seq_t)(m + 1) * (seq_t)(k + 1) * fk2 * W;
        N *= 2;
        Dn = (seq_t)(m + 1) * fk1 * (seq_t)k * (seq_t)k * (seq_t)(k + 2) * (seq_t)(k + 2);
        return N / Dn;
    }
}
