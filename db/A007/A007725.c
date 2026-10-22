/*
 * A007725  Number of spanning trees of Aztec diamonds of order n.
 * https://oeis.org/A007725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(2*n-1) * Product_{1<=j,k<=n-1} (16 - u_j*u_k), u_j =
 *            4*cos(j*Pi/(2*n))^2 = w^j + w^(-j) + 2, w a primitive (2n)-th
 *            root of unity; evaluated exactly in F_p for primes p == 1 (mod
 *            2n) and combined by CRT (Garner); a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   a7725_isprime, a7725_mulm, a7725_powm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a7725_isprime.h"
#include "../utils/a7725_mulm.h"
#include "../utils/a7725_powm.h"

seq_t a(idx_t n) {
    uint64_t L, bits, m, t, q, k, i, jj, j, kk;
    uint64_t qs[64];
    int nq = 0, qi;
    uint64_t *pr, *dg, *pw, *u;
    seq_t x;

    if (n == 0) return 1;
    L = 2 * (uint64_t)n;
    bits = 2 * (2 * (uint64_t)n - 1) + 4 * ((uint64_t)n - 1) * ((uint64_t)n - 1);
    m = bits / 61 + 2;

    t = L;
    for (q = 2; q * q <= t; q++) {
        if (t % q == 0) {
            qs[nq++] = q;
            while (t % q == 0) t /= q;
        }
    }
    if (t > 1) qs[nq++] = t;

    pr = (uint64_t *)malloc(m * sizeof(uint64_t));
    dg = (uint64_t *)malloc(m * sizeof(uint64_t));
    pw = (uint64_t *)malloc(L * sizeof(uint64_t));
    u = (uint64_t *)malloc(((uint64_t)n + 1) * sizeof(uint64_t));

    k = (((uint64_t)1 << 62) - 1) / L;
    for (i = 0; i < m; ) {
        uint64_t p = k * L + 1, g, w = 0, P, A, B, s, c, r;
        k--;
        if (!a7725_isprime(p)) continue;

        for (g = 2; ; g++) {
            bool ok = true;
            w = a7725_powm(g, (p - 1) / L, p);
            for (qi = 0; qi < nq; qi++) {
                if (a7725_powm(w, L / qs[qi], p) == 1) { ok = false; break; }
            }
            if (ok) break;
        }

        pw[0] = 1;
        for (j = 1; j < L; j++) pw[j] = a7725_mulm(pw[j - 1], w, p);
        for (j = 1; j < (uint64_t)n; j++) u[j] = (pw[j] + pw[L - j] + 2) % p;

        A = 1;
        B = 1;
        for (j = 1; j < (uint64_t)n; j++) {
            uint64_t sq = a7725_mulm(u[j], u[j], p);
            A = a7725_mulm(A, (16 + p - sq) % p, p);
            for (kk = j + 1; kk < (uint64_t)n; kk++) {
                uint64_t pr2 = a7725_mulm(u[j], u[kk], p);
                B = a7725_mulm(B, (16 + p - pr2) % p, p);
            }
        }
        P = a7725_powm(4, 2 * (uint64_t)n - 1, p);
        P = a7725_mulm(P, A, p);
        P = a7725_mulm(P, B, p);
        P = a7725_mulm(P, B, p);

        /* Garner mixed-radix digit */
        s = 0;
        c = 1;
        for (jj = 0; jj < i; jj++) {
            s = (s + a7725_mulm(dg[jj] % p, c, p)) % p;
            c = a7725_mulm(c, pr[jj] % p, p);
        }
        r = (P + p - s) % p;
        dg[i] = a7725_mulm(r, a7725_powm(c, p - 2, p), p);
        pr[i] = p;
        i++;
    }

    x = (seq_t)dg[m - 1];
    for (i = m - 1; i-- > 0; ) {
        x = x * (seq_t)pr[i] + (seq_t)dg[i];
    }

    free(pr);
    free(dg);
    free(pw);
    free(u);
    return x;
}
