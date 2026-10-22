/*
 * A005335  Number of labeled nonseparable (or 2-connected) bipartite graphs
 *          with 2n nodes and n nodes in each part.
 * https://oeis.org/A005335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n,n)*A005334(n)/2 = binomial(2n-1,n-1)*h(n,n),
 *            where C(x,y) = log(Sum 2^(p*q) x^p y^q/(p!q!)) (connected
 *            bicolored graphs), log C_x = H_x(x C_x, y C_y), log C_y = H_y(x
 *            C_x, y C_y) (H = blocks), and h(n,n) = (n-1)! n! [x^(n-1) y^n]
 *            L1 * exp(-(n-1)L1 - n L2) * det, L1 = log C_x,...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   is_pr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_pr.h"

static uint64_t PM;
static idx_t WD;
static uint64_t *INV;
#define IX(p, q) ((p) * WD + (q))
#define LIM ((uint64_t)1 << 62)

static uint64_t pw_mod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

static void mlog(const uint64_t *F, uint64_t *G, uint64_t *iG, idx_t R, idx_t S)
{
    idx_t p, q, i, j;
    G[0] = 0;
    iG[0] = 0;
    for (q = 1; q < S; q++) {
        uint64_t s = 0, v;
        for (j = 1; j < q; j++) {
            s += iG[j] * F[q - j];
            if (s >= LIM) s %= PM;
        }
        s %= PM;
        v = ((uint64_t)q * F[q] % PM + PM - s) % PM;
        iG[q] = v;
        G[q] = v * INV[q] % PM;
    }
    for (p = 1; p < R; p++)
        for (q = 0; q < S; q++) {
            uint64_t s = 0, v;
            for (i = 1; i <= p; i++) {
                const uint64_t *gr = iG + IX(i, 0), *fr = F + IX(p - i, 0);
                idx_t jm = (i == p) ? q : q + 1;
                for (j = 0; j < jm; j++) {
                    s += gr[j] * fr[q - j];
                    if (s >= LIM) s %= PM;
                }
            }
            s %= PM;
            v = ((uint64_t)p * F[IX(p, q)] % PM + PM - s) % PM;
            iG[IX(p, q)] = v;
            G[IX(p, q)] = v * INV[p] % PM;
        }
}

static void mexp(const uint64_t *G, uint64_t *F, uint64_t *iG, idx_t R, idx_t S)
{
    idx_t p, q, i, j;
    for (p = 0; p < R; p++)
        for (q = 0; q < S; q++)
            iG[IX(p, q)] = (uint64_t)(p == 0 ? q : p) * G[IX(p, q)] % PM;
    F[0] = 1;
    for (q = 1; q < S; q++) {
        uint64_t s = 0;
        for (j = 1; j <= q; j++) {
            s += iG[j] * F[q - j];
            if (s >= LIM) s %= PM;
        }
        F[q] = s % PM * INV[q] % PM;
    }
    for (p = 1; p < R; p++)
        for (q = 0; q < S; q++) {
            uint64_t s = 0;
            for (i = 1; i <= p; i++) {
                const uint64_t *gr = iG + IX(i, 0), *fr = F + IX(p - i, 0);
                for (j = 0; j <= q; j++) {
                    s += gr[j] * fr[q - j];
                    if (s >= LIM) s %= PM;
                }
            }
            F[IX(p, q)] = s % PM * INV[p] % PM;
        }
}

static void mmul(const uint64_t *X, const uint64_t *Y, uint64_t *Z, idx_t R, idx_t S)
{
    idx_t p, q, i, j;
    for (p = 0; p < R; p++)
        for (q = 0; q < S; q++) {
            uint64_t s = 0;
            for (i = 0; i <= p; i++) {
                const uint64_t *xr = X + IX(i, 0), *yr = Y + IX(p - i, 0);
                for (j = 0; j <= q; j++) {
                    s += xr[j] * yr[q - j];
                    if (s >= LIM) s %= PM;
                }
            }
            Z[IX(p, q)] = s % PM;
        }
}

seq_t a(idx_t n)
{
    idx_t R1 = n + 1, S1 = n + 2, R = n, S = n + 1, sz, p, q, i, j, k, np, t;
    uint64_t *B, *C, *Cx, *Cy, *L1, *L2, *G, *P, *E1, *E2, *F1, *F2, *D, *T, *W1;
    uint64_t *pr, *rs, *dg, *IF;
    uint64_t cand;
    seq_t val, bn;

    WD = S1;
    sz = R1 * S1;
    np = n * n / 29 + 2;
    B = (uint64_t *)calloc(sz, sizeof(uint64_t));
    C = (uint64_t *)calloc(sz, sizeof(uint64_t));
    Cx = (uint64_t *)calloc(sz, sizeof(uint64_t));
    Cy = (uint64_t *)calloc(sz, sizeof(uint64_t));
    L1 = (uint64_t *)calloc(sz, sizeof(uint64_t));
    L2 = (uint64_t *)calloc(sz, sizeof(uint64_t));
    G = (uint64_t *)calloc(sz, sizeof(uint64_t));
    P = (uint64_t *)calloc(sz, sizeof(uint64_t));
    E1 = (uint64_t *)calloc(sz, sizeof(uint64_t));
    E2 = (uint64_t *)calloc(sz, sizeof(uint64_t));
    F1 = (uint64_t *)calloc(sz, sizeof(uint64_t));
    F2 = (uint64_t *)calloc(sz, sizeof(uint64_t));
    D = (uint64_t *)calloc(sz, sizeof(uint64_t));
    T = (uint64_t *)calloc(sz, sizeof(uint64_t));
    W1 = (uint64_t *)calloc(sz, sizeof(uint64_t));
    INV = (uint64_t *)calloc(S1 + 1, sizeof(uint64_t));
    IF = (uint64_t *)calloc(S1 + 1, sizeof(uint64_t));
    pr = (uint64_t *)calloc(np, sizeof(uint64_t));
    rs = (uint64_t *)calloc(np, sizeof(uint64_t));
    dg = (uint64_t *)calloc(np, sizeof(uint64_t));

    cand = ((uint64_t)1 << 30) - 1;
    k = 0;
    while (k < np) {
        if (is_pr(cand)) pr[k++] = cand;
        cand -= 2;
    }

    for (t = 0; t < np; t++) {
        uint64_t M = pr[t], tw, fct, coef, av, bv;
        PM = M;
        INV[1] = 1;
        for (i = 2; i <= S1; i++)
            INV[i] = (M - (M / i) * INV[M % i] % M) % M;
        IF[0] = 1;
        for (i = 1; i <= S1; i++)
            IF[i] = IF[i - 1] * INV[i] % M;

        tw = 1;
        for (p = 0; p < R1; p++) {
            uint64_t c = IF[p];
            for (q = 0; q < S1; q++) {
                B[IX(p, q)] = c * IF[q] % M;
                c = c * tw % M;
            }
            tw = tw * 2 % M;
        }
        mlog(B, C, W1, R1, S1);

        for (p = 0; p < R; p++)
            for (q = 0; q < S; q++) {
                Cx[IX(p, q)] = (uint64_t)(p + 1) * C[IX(p + 1, q)] % M;
                Cy[IX(p, q)] = (uint64_t)(q + 1) * C[IX(p, q + 1)] % M;
            }
        mlog(Cx, L1, W1, R, S);
        mlog(Cy, L2, W1, R, S);

        av = (uint64_t)(n - 1) % M;
        bv = (uint64_t)n % M;
        for (p = 0; p < R; p++)
            for (q = 0; q < S; q++) {
                uint64_t s = (av * L1[IX(p, q)] % M + bv * L2[IX(p, q)] % M) % M;
                G[IX(p, q)] = (M - s) % M;
            }
        mexp(G, P, W1, R, S);

        for (p = 0; p < R; p++)
            for (q = 0; q < S; q++) {
                E1[IX(p, q)] = (uint64_t)p * L1[IX(p, q)] % M;
                E2[IX(p, q)] = (uint64_t)q * L2[IX(p, q)] % M;
                F1[IX(p, q)] = (uint64_t)q * L1[IX(p, q)] % M;
                F2[IX(p, q)] = (uint64_t)p * L2[IX(p, q)] % M;
            }
        E1[0] = (E1[0] + 1) % M;
        E2[0] = (E2[0] + 1) % M;
        mmul(E1, E2, D, R, S);
        mmul(F1, F2, T, R, S);
        for (p = 0; p < R; p++)
            for (q = 0; q < S; q++)
                D[IX(p, q)] = (D[IX(p, q)] + M - T[IX(p, q)]) % M;
        mmul(L1, D, T, R, S);

        coef = 0;
        for (i = 0; i < R; i++)
            for (j = 0; j < S; j++) {
                coef += P[IX(i, j)] * T[IX(n - 1 - i, n - j)];
                if (coef >= LIM) coef %= M;
            }
        coef %= M;

        fct = 1;
        for (i = 1; i < n; i++) fct = fct * (uint64_t)i % M;
        fct = fct * fct % M * ((uint64_t)n % M) % M;
        rs[t] = coef * fct % M;
    }

    for (i = 0; i < np; i++) {
        uint64_t M = pr[i], x = rs[i];
        for (j = 0; j < i; j++) {
            x = (x + M - dg[j] % M) % M;
            x = x * pw_mod(pr[j] % M, M - 2, M) % M;
        }
        dg[i] = x;
    }
    val = (seq_t)dg[np - 1];
    for (i = np - 1; i > 0; i--)
        val = val * (seq_t)pr[i - 1] + (seq_t)dg[i - 1];

    bn = 1;
    for (i = 1; i < n; i++)
        bn = bn * (seq_t)(n + i) / (seq_t)i;

    free(B); free(C); free(Cx); free(Cy); free(L1); free(L2);
    free(G); free(P); free(E1); free(E2); free(F1); free(F2);
    free(D); free(T); free(W1); free(INV); free(IF);
    free(pr); free(rs); free(dg);
    return val * bn;
}
