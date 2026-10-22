/*
 * A003957  The Dottie number: decimal expansion of root of cos(x) = x.
 * https://oeis.org/A003957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (n+1)-th decimal digit after the point of the root d of
 *            cos(d) = d. d is found by Newton's method x <- x + (cos x -
 *            x)/(1 + sin x) in binary fixed-point, doubling the precision at
 *            each step; sin and cos come from Taylor series on x/2^r followed
 *            by r angle doublings
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t limb;

/* fixed-point numbers: v[0] is the integer part, v[i] has weight 2^(-32 i) */
static limb *g_tmp, *X, *Y, *S, *V, *C, *B, *E, *D, *T, *T2, *TERM, *P, *Q, *ONE;

static int bfirst(const limb *a, int w)
{
    int i;
    for (i = 0; i <= w; i++)
        if (a[i]) return i;
    return w + 1;
}

static void bmul(limb *r, const limb *a, const limb *b, int w)
{
    int K = w + 2, k, i;
    int za = bfirst(a, w), zb = bfirst(b, w);
    unsigned __int128 c = 0;
    for (k = K; k >= 0; k--) {
        int lo = k - w > za ? k - w : za;
        int hi = k - zb < w ? k - zb : w;
        unsigned __int128 s = c;
        for (i = lo; i <= hi; i++) s += (uint64_t)a[i] * b[k - i];
        g_tmp[k] = (limb)s;
        c = s >> 32;
    }
    memcpy(r, g_tmp, (size_t)(w + 1) * sizeof(limb));
}

static void badd(limb *r, const limb *a, const limb *b, int w)
{
    uint64_t c = 0;
    int i;
    for (i = w; i >= 0; i--) {
        c += (uint64_t)a[i] + b[i];
        r[i] = (limb)c;
        c >>= 32;
    }
}

static void bsub(limb *r, const limb *a, const limb *b, int w)
{
    uint64_t br = 0;
    int i;
    for (i = w; i >= 0; i--) {
        uint64_t d = (uint64_t)a[i] - b[i] - br;
        r[i] = (limb)d;
        br = (d >> 63) & 1;
    }
}

static int bcmp(const limb *a, const limb *b, int w)
{
    int i;
    for (i = 0; i <= w; i++)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

static void bmuls(limb *r, const limb *a, uint32_t m, int w)
{
    uint64_t c = 0;
    int i;
    for (i = w; i >= 0; i--) {
        c += (uint64_t)a[i] * m;
        r[i] = (limb)c;
        c >>= 32;
    }
}

static void bdivs(limb *r, const limb *a, uint32_t d, int w)
{
    uint64_t rem = 0;
    int i;
    for (i = 0; i <= w; i++) {
        uint64_t cur = (rem << 32) | a[i];
        r[i] = (limb)(cur / d);
        rem = cur % d;
    }
}

/* S = sin X, V = 1 - cos X */
static void sincos_x(int w)
{
    int R = w < 64 ? 1 : (w < 256 ? 2 : 4);
    int i, k, sg, j;
    for (i = w; i >= 0; i--) T[i] = i >= R ? X[i - R] : 0;
    bmul(T2, T, T, w);

    memcpy(S, T, (size_t)(w + 1) * sizeof(limb));
    memcpy(TERM, T, (size_t)(w + 1) * sizeof(limb));
    for (k = 1, sg = 1;; k += 2, sg ^= 1) {
        bmul(TERM, TERM, T2, w);
        bdivs(TERM, TERM, (uint32_t)((k + 1) * (k + 2)), w);
        if (bfirst(TERM, w) > w) break;
        if (sg) bsub(S, S, TERM, w); else badd(S, S, TERM, w);
    }

    bdivs(TERM, T2, 2, w);
    memcpy(V, TERM, (size_t)(w + 1) * sizeof(limb));
    for (k = 2, sg = 1;; k += 2, sg ^= 1) {
        bmul(TERM, TERM, T2, w);
        bdivs(TERM, TERM, (uint32_t)((k + 1) * (k + 2)), w);
        if (bfirst(TERM, w) > w) break;
        if (sg) bsub(V, V, TERM, w); else badd(V, V, TERM, w);
    }

    for (j = 0; j < 32 * R; j++) {
        bmul(P, S, V, w);
        bmul(Q, V, V, w);
        bsub(S, S, P, w);
        badd(S, S, S, w);      /* sin 2t = 2 s (1 - v) */
        badd(V, V, V, w);
        bsub(V, V, Q, w);
        badd(V, V, V, w);      /* 1 - cos 2t = 2 v (2 - v) */
    }
}

static void newton(int w)
{
    sincos_x(w);
    bsub(C, ONE, V, w);        /* cos x */
    badd(B, ONE, S, w);        /* 1 + sin x */
    /* refine Y ~ 1/B */
    bmul(E, B, Y, w);
    if (bcmp(E, ONE, w) <= 0) {
        bsub(D, ONE, E, w); bmul(D, Y, D, w); badd(Y, Y, D, w);
    } else {
        bsub(D, E, ONE, w); bmul(D, Y, D, w); bsub(Y, Y, D, w);
    }
    if (bcmp(C, X, w) >= 0) {
        bsub(D, C, X, w); bmul(D, D, Y, w); badd(X, X, D, w);
    } else {
        bsub(D, X, C, w); bmul(D, D, Y, w); bsub(X, X, D, w);
    }
}

static unsigned char *dig = NULL;
static long ndig = 0;

static void compute(long nd)
{
    const int G = 10;
    int acc = (int)((double)(nd + 20) * 3.3219280948873623 / 32.0) + 2;
    int W = acc + G;
    size_t sz = (size_t)(W + 3) * sizeof(limb);
    limb **arrs[15];
    int na = 15, i, lv[64], cnt = 0, a;
    double f;
    long pos;

    arrs[0] = &g_tmp; arrs[1] = &X; arrs[2] = &Y; arrs[3] = &S; arrs[4] = &V;
    arrs[5] = &C; arrs[6] = &B; arrs[7] = &E; arrs[8] = &D; arrs[9] = &T;
    arrs[10] = &T2; arrs[11] = &TERM; arrs[12] = &P; arrs[13] = &Q; arrs[14] = &ONE;
    for (i = 0; i < na; i++) *arrs[i] = (limb *)calloc((size_t)(W + 3), sizeof(limb));
    (void)sz;
    ONE[0] = 1;

    f = 0.7390851332151607;
    for (i = 1; i <= 2; i++) { f *= 4294967296.0; X[i] = (limb)f; f -= (double)X[i]; }
    f = 0.5975110;
    for (i = 1; i <= 2; i++) { f *= 4294967296.0; Y[i] = (limb)f; f -= (double)Y[i]; }

    a = acc;
    while (a > 6) { lv[cnt++] = a; a = a / 2 + 2; }
    lv[cnt++] = a;

    for (i = 0; i < 4; i++) newton(lv[cnt - 1] + G);
    for (i = cnt - 2; i >= 0; i--) newton(lv[i] + G);

    free(dig);
    dig = (unsigned char *)malloc((size_t)nd + 16);
    X[0] = 0;
    for (pos = 0; pos < nd; pos += 9) {
        uint32_t blk;
        int j;
        bmuls(X, X, 1000000000u, W);
        blk = X[0];
        X[0] = 0;
        for (j = 8; j >= 0; j--) {
            if (pos + j < nd) dig[pos + j] = (unsigned char)(blk % 10);
            blk /= 10;
        }
    }
    ndig = nd;

    for (i = 0; i < na; i++) { free(*arrs[i]); *arrs[i] = NULL; }
}

seq_t a(idx_t n)
{
    seq_t r = 0;
    if ((long)n >= ndig) {
        long target = (long)n + 1;
        if (target < 2 * ndig) target = 2 * ndig;
        if (target < 100) target = 100;
        compute(target);
    }
    r += (int)dig[n];
    return r;
}
