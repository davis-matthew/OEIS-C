/*
 * A002178  Numerators of Cotesian numbers (not in lowest terms):
 *          A002176*C(n,1).
 * https://oeis.org/A002178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A002176(n)*C(n,1), where C(n,k) = (1/n) *
 *            Integral_{t=0..n} Prod_{j=0..n, j<>k} (t-j)/(k-j) dt are the
 *            Newton-Cotes numbers and A002176(n) = lcm of their reduced
 *            denominators. Exact arithmetic: with P(t) = Prod_{j=0..n} (t-j),
 *            C(n,k) = (-1)^(n-k) * A_k / ((n+1)! k! (n-k)!), where A_k =
 *            (n+...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   leg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/leg.h"

static size_t W;

static void bmuls(uint32_t *x, uint64_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < W; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void badd(uint32_t *x, const uint32_t *y) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < W; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void bsub(uint32_t *x, const uint32_t *y) {
    uint64_t c = 1;
    size_t i;
    for (i = 0; i < W; i++) {
        uint64_t t = (uint64_t)x[i] + (uint32_t)~y[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void bneg(uint32_t *x) {
    uint64_t c = 1;
    size_t i;
    for (i = 0; i < W; i++) {
        uint64_t t = (uint64_t)(uint32_t)~x[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void bmul(uint32_t *res, const uint32_t *a, const uint32_t *b) {
    size_t i, j;
    memset(res, 0, W * sizeof(uint32_t));
    for (i = 0; i < W; i++) {
        uint64_t c = 0;
        if (a[i] == 0) continue;
        for (j = 0; i + j < W; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + res[i + j] + c;
            res[i + j] = (uint32_t)t;
            c = t >> 32;
        }
    }
}

static uint32_t bdiv(uint32_t *x, uint32_t d) {
    uint64_t r = 0;
    size_t i = W;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    return (uint32_t)r;
}

static int bzero(const uint32_t *x) {
    size_t i;
    for (i = 0; i < W; i++) if (x[i]) return 0;
    return 1;
}

/* computes A_k into out (two's complement), using G[m] */
static void compute_A(uint64_t n, uint64_t k, uint32_t *G, uint32_t *u, uint32_t *out) {
    uint64_t m;
    memset(u, 0, W * sizeof(uint32_t));
    memset(out, 0, W * sizeof(uint32_t));
    u[0] = 1;
    out[0] = 1;
    m = n;
    while (m > 0) {
        m--;
        bmuls(u, k * (m + 2));
        badd(u, G + m * W);
        bmuls(out, n * (m + 1));
        badd(out, u);
    }
}

seq_t a(idx_t n) {
    uint64_t lg = 1, bits, j, i, k, m, np = 0;
    uint32_t *p, *G, *E, *tmp, *u, *A, *mag, *mag1;
    char *comp;
    uint64_t *pr;
    long long *emax, *vN1;
    seq_t r = 0;

    while (((uint64_t)1 << lg) < n + 2) lg++;
    bits = (3 * (uint64_t)n + 6) * lg + 64;
    W = (size_t)(bits / 32 + 2);

    p = (uint32_t *)calloc((size_t)(n + 2) * W, sizeof(uint32_t));
    G = (uint32_t *)calloc((size_t)(n + 1) * W, sizeof(uint32_t));
    E = (uint32_t *)calloc(W, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(W, sizeof(uint32_t));
    u = (uint32_t *)calloc(W, sizeof(uint32_t));
    A = (uint32_t *)calloc(W, sizeof(uint32_t));
    mag = (uint32_t *)calloc(W, sizeof(uint32_t));
    mag1 = (uint32_t *)calloc(W, sizeof(uint32_t));

    /* P(t) = prod_{j=0..n} (t - j) */
    p[0] = 1;
    for (j = 0; j <= n; j++) {
        i = j + 2;
        while (i > 0) {
            i--;
            memcpy(tmp, p + i * W, W * sizeof(uint32_t));
            bmuls(tmp, j);
            if (i > 0) memcpy(p + i * W, p + (i - 1) * W, W * sizeof(uint32_t));
            else memset(p, 0, W * sizeof(uint32_t));
            bsub(p + i * W, tmp);
        }
    }

    /* G_m = p_{m+1} * E_{m+1}, E_m = (n+1)!/m! */
    E[0] = 1;
    m = n + 1;
    while (m > 0) {
        m--;
        bmul(G + m * W, p + (m + 1) * W, E);
        bmuls(E, m + 1);
    }

    /* primes up to n+1 */
    comp = (char *)calloc((size_t)(n + 2), 1);
    pr = (uint64_t *)malloc((size_t)(n + 2) * sizeof(uint64_t));
    for (i = 2; i <= n + 1; i++) {
        if (comp[i]) continue;
        pr[np++] = i;
        for (j = i * i; j <= n + 1; j += i) comp[j] = 1;
    }
    emax = (long long *)calloc((size_t)(np + 1), sizeof(long long));
    vN1 = (long long *)calloc((size_t)(np + 1), sizeof(long long));

    for (k = 0; k <= n; k++) {
        int isone = (k == 1);
        if (!(k <= n / 2 || isone)) break;
        compute_A(n, k, G, u, A);
        if (A[W - 1] >> 31) bneg(A);
        if (isone) memcpy(mag1, A, W * sizeof(uint32_t));
        for (i = 0; i < np; i++) {
            uint64_t q = pr[i];
            long long vN = leg(n + 1, q) + leg(k, q) + leg(n - k, q);
            long long vA = 0, e;
            if (isone) vN1[i] = vN;
            if (vN == 0) continue;
            if (bzero(A)) {
                vA = vN;
            } else {
                memcpy(mag, A, W * sizeof(uint32_t));
                while (vA < vN && bdiv(mag, (uint32_t)q) == 0) vA++;
            }
            e = vN - (vA < vN ? vA : vN);
            if (e > emax[i]) emax[i] = e;
        }
    }

    for (i = 0; i < np; i++) {
        long long f = emax[i] - vN1[i];
        while (f < 0) { bdiv(mag1, (uint32_t)pr[i]); f++; }
    }
    for (i = 0; i < np; i++) {
        long long f = emax[i] - vN1[i];
        while (f > 0) { bmuls(mag1, pr[i]); f--; }
    }

    {
        size_t top = W;
        while (top > 0 && mag1[top - 1] == 0) top--;
        while (top > 0) {
            int b;
            top--;
            for (b = 3; b >= 0; b--) {
                r = (seq_t)(r * 256);
                r += (seq_t)((mag1[top] >> (8 * b)) & 255u);
            }
        }
    }

    free(p); free(G); free(E); free(tmp); free(u); free(A); free(mag); free(mag1);
    free(comp); free(pr); free(emax); free(vN1);
    return r;
}
