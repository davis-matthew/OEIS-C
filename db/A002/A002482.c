/*
 * A002482  Theta series of Borcherds' 27-dimensional unimodular lattice U_27.
 * https://oeis.org/A002482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] ( theta_3(q)^27 - 54*theta_3(q)^19*D(q) +
 *            216*theta_3(q)^11*D(q)^2 ), where theta_3(q) = Sum_{k in Z}
 *            q^(k^2) and D(q) = q*Product_{k>=1} (1-q^(2k-1))^8*(1-q^(4k))^8
 *            = q*psi(-q)^8, psi(q) = Sum_{k>=0} q^(k(k+1)/2). (Conway-Sloane
 *            form for odd unimodular lattices; a(1)=a(2)=0 and a(3)...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   a2482_isprime, a2482_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2482_isprime.h"
#include "../utils/a2482_powmod.h"

#define A2482_ACC(acc, x) do { (acc) += (x); if ((acc) >> 62) (acc) %= p; } while (0)

static idx_t a2482_N = 0;
static int a2482_np = 0;
static uint64_t a2482_P[64];
static uint32_t *a2482_res = NULL;

/* G = f^a * g^b mod p, f = theta_3, g = psi(-q); writes G[0..N] */
static void a2482_series(uint64_t p, idx_t N, uint64_t a, uint64_t b, const uint64_t *inv,
                         const idx_t *fi, const uint64_t *fv, const uint64_t *fd, idx_t nf,
                         const idx_t *gi, const uint64_t *gv, const uint64_t *gd, idx_t ng,
                         uint64_t *G, uint64_t *DG, uint64_t *U, uint64_t *V, uint64_t *W,
                         uint64_t *fda, uint64_t *gdb) {
    idx_t m, t;
    for (t = 0; t < nf; t++) fda[t] = a % p * fd[t] % p;
    for (t = 0; t < ng; t++) gdb[t] = b % p * gd[t] % p;
    G[0] = 1; DG[0] = 0; U[0] = 0; V[0] = 1; W[0] = 0;
    for (m = 1; m <= N; m++) {
        uint64_t up = 0, w = 0, r1 = 0, r2 = 0, lr = 0, v, rhs, lhs, dg;
        for (t = 1; t < ng && gi[t] <= m; t++) {
            idx_t k = m - gi[t];
            A2482_ACC(up, gv[t] * DG[k]);
            A2482_ACC(w, gdb[t] * G[k]);
        }
        up %= p; w %= p;
        W[m] = w;
        for (t = 1; t < nf && fi[t] <= m; t++) {
            idx_t k = m - fi[t];
            A2482_ACC(r1, fda[t] * V[k]);
            A2482_ACC(r2, fv[t] * W[k]);
            A2482_ACC(lr, fv[t] * U[k]);
        }
        r1 %= p; r2 %= p; lr %= p;
        rhs = (r1 + r2 + w) % p;
        lhs = (up + lr) % p;
        dg = (rhs + p - lhs) % p;
        DG[m] = dg;
        G[m] = dg * inv[m] % p;
        U[m] = (dg + up) % p;
        v = G[m];
        for (t = 1; t < ng && gi[t] <= m; t++) A2482_ACC(v, gv[t] * G[m - gi[t]]);
        V[m] = v % p;
    }
}

static void a2482_compute(idx_t N) {
    idx_t nf = 0, ng = 0, t, m;
    int lg = 0, bits, np, i;
    uint64_t cand;
    idx_t L = N + 1;
    idx_t *fi, *gi;
    uint64_t *fv, *fd, *gv, *gd, *fda, *gdb, *G, *DG, *U, *V, *W, *R, *inv;

    while ((((uint64_t)1) << lg) < (uint64_t)N + 2) lg++;
    bits = 14 * lg + 60;
    np = bits / 27 + 1;
    if (np > 64) np = 64;
    cand = (((uint64_t)1) << 28) - 1;
    for (i = 0; i < np; i++) {
        while (!a2482_isprime(cand)) cand -= 2;
        a2482_P[i] = cand;
        cand -= 2;
    }

    while ((idx_t)nf * nf <= N) nf++;
    while ((idx_t)ng * (ng + 1) / 2 <= N) ng++;
    fi = (idx_t *)malloc(sizeof(idx_t) * nf);
    gi = (idx_t *)malloc(sizeof(idx_t) * ng);
    fv = (uint64_t *)malloc(sizeof(uint64_t) * nf);
    fd = (uint64_t *)malloc(sizeof(uint64_t) * nf);
    fda = (uint64_t *)malloc(sizeof(uint64_t) * nf);
    gv = (uint64_t *)malloc(sizeof(uint64_t) * ng);
    gd = (uint64_t *)malloc(sizeof(uint64_t) * ng);
    gdb = (uint64_t *)malloc(sizeof(uint64_t) * ng);
    G = (uint64_t *)malloc(sizeof(uint64_t) * L);
    DG = (uint64_t *)malloc(sizeof(uint64_t) * L);
    U = (uint64_t *)malloc(sizeof(uint64_t) * L);
    V = (uint64_t *)malloc(sizeof(uint64_t) * L);
    W = (uint64_t *)malloc(sizeof(uint64_t) * L);
    R = (uint64_t *)malloc(sizeof(uint64_t) * L);
    inv = (uint64_t *)malloc(sizeof(uint64_t) * (L + 1));
    for (t = 0; t < nf; t++) fi[t] = t * t;
    for (t = 0; t < ng; t++) gi[t] = t * (t + 1) / 2;

    free(a2482_res);
    a2482_res = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)np * L);

    for (i = 0; i < np; i++) {
        uint64_t p = a2482_P[i];
        inv[1] = 1;
        for (m = 2; m <= N; m++) inv[m] = (p - (p / m) * inv[p % m] % p) % p;
        fv[0] = 1; fd[0] = 0;
        for (t = 1; t < nf; t++) { fv[t] = 2; fd[t] = (2 * (uint64_t)fi[t]) % p; }
        for (t = 0; t < ng; t++) {
            gv[t] = (gi[t] & 1) ? p - 1 : 1;
            gd[t] = (uint64_t)gi[t] % p * gv[t] % p;
        }
        a2482_series(p, N, 27, 0, inv, fi, fv, fd, nf, gi, gv, gd, ng, G, DG, U, V, W, fda, gdb);
        for (m = 0; m <= N; m++) R[m] = G[m];
        a2482_series(p, N, 19, 8, inv, fi, fv, fd, nf, gi, gv, gd, ng, G, DG, U, V, W, fda, gdb);
        for (m = 1; m <= N; m++) R[m] = (R[m] + (p - 54) * G[m - 1]) % p;
        a2482_series(p, N, 11, 16, inv, fi, fv, fd, nf, gi, gv, gd, ng, G, DG, U, V, W, fda, gdb);
        for (m = 2; m <= N; m++) R[m] = (R[m] + 216 * G[m - 2]) % p;
        for (m = 0; m <= N; m++) a2482_res[(size_t)i * L + m] = (uint32_t)R[m];
    }

    free(fi); free(gi); free(fv); free(fd); free(fda); free(gv); free(gd); free(gdb);
    free(G); free(DG); free(U); free(V); free(W); free(R); free(inv);
    a2482_N = N;
    a2482_np = np;
}

seq_t a(idx_t n) {
    uint64_t d[64];
    int i, j;
    seq_t r;
    if (a2482_res == NULL || n > a2482_N) {
        idx_t N = a2482_N + a2482_N / 2;
        if (N < n) N = n;
        if (N < 256) N = 256;
        a2482_compute(N);
    }
    for (i = 0; i < a2482_np; i++) {
        uint64_t p = a2482_P[i];
        uint64_t x = a2482_res[(size_t)i * (a2482_N + 1) + n];
        uint64_t cur = 0, mult = 1;
        for (j = 0; j < i; j++) {
            cur = (cur + d[j] % p * mult) % p;
            mult = mult * (a2482_P[j] % p) % p;
        }
        d[i] = (x + p - cur) % p * a2482_powmod(mult, p - 2, p) % p;
    }
    r = 0;
    for (i = a2482_np - 1; i >= 0; i--)
        r = r * (seq_t)a2482_P[i] + (seq_t)d[i];
    return r;
}
