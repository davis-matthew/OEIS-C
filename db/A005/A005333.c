/*
 * A005333  Number of 2-colored connected labeled graphs with n vertices of
 *          the first color and n vertices of the second color.
 * https://oeis.org/A005333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n,n) where c(0,1) = 1, c(0,m) = 0, c(n,m) = 2^(n*m) -
 *            Sum_{1 <= k <= n, 0 <= j <= m, k < n or j < m} C(n-1,k-1) *
 *            C(m,j) * 2^((n-k)*(m-j)) * c(k,j). Rescaled as F(k,m) =
 *            c(k,m)/((k-1)! m!), R(d,t) = 2^(d*t)/(d! t!): F(k,m) = k*R(k,m)
 *            - Sum F(i,j) R(k-i,m-j); computed modulo several prime...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   seq_is_fixed_width, trial_prime, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/trial_prime.h"

static uint64_t pw_mod(uint64_t b, uint64_t e, uint64_t p) {
    uint64_t r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

static uint64_t residue(idx_t n, uint64_t p) {
    idx_t w = n + 1, k, m, i, j, d, t;
    uint64_t *fact = (uint64_t *)malloc(w * sizeof(uint64_t));
    uint64_t *ifact = (uint64_t *)malloc(w * sizeof(uint64_t));
    uint64_t *R = (uint64_t *)malloc(w * w * sizeof(uint64_t));
    uint64_t *F = (uint64_t *)calloc(w * w, sizeof(uint64_t));
    uint64_t res;

    fact[0] = 1;
    for (i = 1; i <= n; i++) fact[i] = fact[i - 1] * (i % p) % p;
    ifact[n] = pw_mod(fact[n], p - 2, p);
    for (i = n; i > 0; i--) ifact[i - 1] = ifact[i] * (i % p) % p;

    for (d = 0; d <= n; d++) {
        uint64_t s = pw_mod(2, d, p), e = 1;
        for (t = 0; t <= n; t++) {
            R[d * w + t] = e * ifact[d] % p * ifact[t] % p;
            e = e * s % p;
        }
    }

    for (k = 1; k <= n; k++) {
        for (m = 0; m <= n; m++) {
            unsigned __int128 acc = 0;
            for (i = 1; i <= k; i++) {
                const uint64_t *Fi = F + i * w;
                const uint64_t *Rd = R + (k - i) * w;
                idx_t jm = (i == k) ? m : m + 1;
                for (j = 0; j < jm; j++)
                    acc += (unsigned __int128)Fi[j] * Rd[m - j];
            }
            uint64_t s = (uint64_t)(acc % p);
            uint64_t base = (k % p) * R[k * w + m] % p;
            F[k * w + m] = (base + p - s) % p;
        }
    }

    res = F[n * w + n] * fact[n - 1] % p * fact[n] % p;
    free(fact);
    free(ifact);
    free(R);
    free(F);
    return res;
}

seq_t a(idx_t n) {
    idx_t P = n * n / 31 + 1, i, j;
    uint64_t *pr, *r, *v, cand;
    seq_t x;

    if (seq_is_fixed_width() && P > 5) P = 5;

    pr = (uint64_t *)malloc(P * sizeof(uint64_t));
    r = (uint64_t *)malloc(P * sizeof(uint64_t));
    v = (uint64_t *)malloc(P * sizeof(uint64_t));

    cand = 4294967295ULL;
    for (i = 0; i < P; i++) {
        while (!trial_prime(cand)) cand -= 2;
        pr[i] = cand;
        cand -= 2;
    }

    for (i = 0; i < P; i++) r[i] = residue(n, pr[i]);

    for (i = 0; i < P; i++) {
        uint64_t p = pr[i], tt = r[i] % p;
        for (j = 0; j < i; j++) {
            tt = (tt + p - v[j] % p) % p;
            tt = tt * pw_mod(pr[j] % p, p - 2, p) % p;
        }
        v[i] = tt;
    }

    if (seq_is_fixed_width()) {
        unsigned __int128 y = v[P - 1];
        for (i = P - 1; i > 0; i--)
            y = y * pr[i - 1] + v[i - 1];
        x = u128_to_seq(y);
    } else {
        x = (seq_t)(idx_t)v[P - 1];
        for (i = P - 1; i > 0; i--)
            x = x * (seq_t)(idx_t)pr[i - 1] + (seq_t)(idx_t)v[i - 1];
    }

    free(pr);
    free(r);
    free(v);
    return x;
}
