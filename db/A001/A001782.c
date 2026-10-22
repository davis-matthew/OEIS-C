/*
 * A001782  Discriminants of Shapiro polynomials.
 * https://oeis.org/A001782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(n) = discrim(P_n(x)), where P_0 = Q_0 = 1, P_{k+1} = P_k +
 *            x^(2^k) Q_k, Q_{k+1} = P_k - x^(2^k) Q_k. Computed as
 *            (-1)^(d(d-1)/2) * Res(P_n, P_n') / lc(P_n), d = 2^n - 1: the
 *            resultant is taken modulo many 31-bit primes and the result
 *            rebuilt by balanced mixed-radix CRT (Garner)
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 3, i64 n <= 3, i128 n <= 4,
 *            big any n
 * Helpers:   a1782_disc_mod, a1782_isprime, a1782_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a1782_disc_mod.h"
#include "../utils/a1782_isprime.h"
#include "../utils/a1782_pw.h"

seq_t a(idx_t n) {
    idx_t len = 1, j;
    long long d, i, np, bits;
    int bl = 0;
    long long *P, *Q;
    uint64_t *f, *g, *pr, *rs;
    long long *v;
    uint64_t cand;
    seq_t x = 0;

    P = (long long *)malloc(sizeof(long long) << n);
    Q = (long long *)malloc(sizeof(long long) << n);
    P[0] = 1; Q[0] = 1;
    for (j = 0; j < n; j++) {
        idx_t m;
        for (m = 0; m < len; m++) {
            long long pj = P[m], qj = Q[m];
            P[len + m] = qj;
            Q[len + m] = -qj;
            Q[m] = pj;
        }
        len *= 2;
    }
    d = (long long)len - 1;

    { long long t = d; while (t) { bl++; t >>= 1; } }
    bits = (d - 1) * (long long)n / 2 + d * (3 * bl + 1) / 2 + 64;
    np = bits / 30 + 2;

    f = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(d + 1));
    g = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(d + 1));
    pr = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)np);
    rs = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)np);
    v = (long long *)malloc(sizeof(long long) * (size_t)np);

    cand = 2147483647ULL;
    for (i = 0; i < np; i++) {
        while (!a1782_isprime(cand)) cand -= 2;
        pr[i] = cand;
        cand -= 2;
        rs[i] = a1782_disc_mod(P, d, pr[i], f, g);
    }

    for (i = 0; i < np; i++) {
        uint64_t p = pr[i], t = rs[i];
        long long k;
        for (k = 0; k < i; k++) {
            long long vk = v[k] % (long long)p;
            uint64_t vm = (uint64_t)((vk + (long long)p) % (long long)p);
            t = (t + p - vm) % p;
            t = t * a1782_pw(pr[k] % p, p - 2, p) % p;
        }
        v[i] = (t > p / 2) ? (long long)t - (long long)p : (long long)t;
    }

    for (i = np - 1; i >= 0; i--) {
        x = x * (idx_t)pr[i];
        if (v[i] >= 0) x += (idx_t)v[i];
        else x -= (idx_t)(-v[i]);
    }

    free(P); free(Q); free(f); free(g); free(pr); free(rs); free(v);
    return x;
}
