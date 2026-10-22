/*
 * A004796  Numbers k such that if 2 <= j < k then the fractional part of the
 *          k-th partial sum of the harmonic series is < the fractional part
 *          of the j-th partial sum of the harmonic series.
 * https://oeis.org/A004796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. For m >= 2 let k_m be the least k with H(k) > m; only
 *            such k can set records. a(n) is the n-th k_m whose excess H(k_m)
 *            - m is smaller than every earlier one. Small k are handled by
 *            direct summation. For large k the code uses k_m = ceil(y - 1/2),
 *            with y = exp(m - gamma) - exp(gamma - m)/24...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 31, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef uint32_t limb;

static limb *scr = NULL;
static size_t scrn = 0;

static void b_add(limb *a, const limb *b, int L) {
    uint64_t c = 0;
    for (int i = 0; i < L; i++) { c += (uint64_t)a[i] + b[i]; a[i] = (limb)c; c >>= 32; }
}
static void b_sub(limb *a, const limb *b, int L) {
    uint64_t br = 0;
    for (int i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] - b[i] - br;
        a[i] = (limb)t; br = (t >> 63) & 1;
    }
}
static int b_cmp(const limb *a, const limb *b, int L) {
    for (int i = L - 1; i >= 0; i--)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}
static int b_zero(const limb *a, int L) {
    for (int i = 0; i < L; i++) if (a[i]) return 0;
    return 1;
}
static void b_muls(limb *a, uint32_t s, int L) {
    uint64_t c = 0;
    for (int i = 0; i < L; i++) { c += (uint64_t)a[i] * s; a[i] = (limb)c; c >>= 32; }
}
static void b_divs(limb *a, uint32_t s, int L) {
    uint64_t r = 0;
    for (int i = L - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | a[i];
        a[i] = (limb)(cur / s); r = cur % s;
    }
}
static void b_shl1(limb *a, int L) {
    limb c = 0;
    for (int i = 0; i < L; i++) { limb nc = a[i] >> 31; a[i] = (a[i] << 1) | c; c = nc; }
}
/* r = (a*b) >> (32*sh), low Lr limbs; partial products far below the cut are skipped */
static void b_mul(limb *r, int Lr, const limb *a, int La, const limb *b, int Lb, int sh) {
    size_t need = (size_t)La + Lb;
    if (need > scrn) { scr = (limb *)realloc(scr, need * sizeof(limb)); scrn = need; }
    memset(scr, 0, need * sizeof(limb));
    int lo = sh - 3;
    for (int i = 0; i < La; i++) {
        if (!a[i]) continue;
        int j0 = lo - i; if (j0 < 0) j0 = 0;
        uint64_t c = 0;
        for (int j = j0; j < Lb; j++) {
            uint64_t cur = (uint64_t)a[i] * b[j] + scr[i + j] + c;
            scr[i + j] = (limb)cur; c = cur >> 32;
        }
        scr[i + Lb] = (limb)c;
    }
    for (int k = 0; k < Lr; k++) {
        size_t s = (size_t)k + sh;
        r[k] = s < need ? scr[s] : 0;
    }
}

static seq_t *res = NULL;
static idx_t cnt = 0, cap = 0;
static long double minlog;
static int lastm = 0;
static int inited = 0;
static const long double GAM = 0.57721566490153286060651209L;

static void rec(long double lg, seq_t k) {
    if (lg < minlog) {
        minlog = lg;
        if (cnt == cap) {
            cap = cap ? 2 * cap : 64;
            res = (seq_t *)realloc(res, cap * sizeof(seq_t));
        }
        res[cnt++] = k;
    }
}

static void direct(void) {
    long double H = 1.0L, comp = 0.0L;
    int mcur = 2;
    minlog = logl(0.5L);
    for (uint64_t k = 2; k <= 2000000; k++) {
        long double y = 1.0L / (long double)k - comp;
        long double t = H + y;
        comp = (t - H) - y;
        H = t;
        if (H > (long double)mcur) {
            rec(logl(H - (long double)mcur), (seq_t)(idx_t)k);
            mcur++;
        }
    }
    lastm = mcur - 1;
}

static void extend(int Mmax) {
    int FC = (int)(Mmax * 1.4427) / 32 + 5;
    int FG = FC + 2, LG = FG + 1;
    int IX = (int)(Mmax * 1.4427) / 32 + 3, LX = IX + FC;

    /* ln 2 = 2 atanh(1/3) */
    limb *p = (limb *)calloc(LG, sizeof(limb));
    limb *ln2 = (limb *)calloc(LG, sizeof(limb));
    limb *t = (limb *)calloc(LG, sizeof(limb));
    p[FG] = 1; b_divs(p, 3, LG);
    for (uint32_t i = 1; !b_zero(p, LG); i += 2) {
        memcpy(t, p, LG * sizeof(limb));
        b_divs(t, i, LG);
        b_add(ln2, t, LG);
        b_divs(p, 9, LG);
    }
    b_muls(ln2, 2, LG);

    /* Brent-McMillan: gamma = S/V - ln N */
    uint32_t N = 1; uint32_t jexp = 0;
    while (577.0 * N < 100.0 * (32.0 * FG + 64)) { N *= 2; jexp++; }
    int IG = (int)((2.886 * N + 64) / 32) + 2;
    int LB = IG + FG;
    limb *B = (limb *)calloc(LB + 1, sizeof(limb));
    limb *C = (limb *)calloc(LB + 1, sizeof(limb));
    limb *S = (limb *)calloc(LB + 1, sizeof(limb));
    limb *V = (limb *)calloc(LB + 1, sizeof(limb));
    B[FG] = 1; V[FG] = 1;
    uint32_t N2 = N * N;
    for (uint32_t k = 1;; k++) {
        b_muls(B, N2, LB); b_divs(B, k, LB); b_divs(B, k, LB);
        b_muls(C, N2, LB); b_divs(C, k, LB); b_add(C, B, LB); b_divs(C, k, LB);
        b_add(S, C, LB); b_add(V, B, LB);
        if (b_zero(B, LB) && b_zero(C, LB)) break;
    }
    limb *q = (limb *)calloc(LG, sizeof(limb));
    {
        uint32_t qi = 0;
        while (b_cmp(S, V, LB + 1) >= 0) { b_sub(S, V, LB + 1); qi++; }
        q[FG] = qi;
        for (long bit = 32L * FG - 1; bit >= 0; bit--) {
            b_shl1(S, LB + 1);
            if (b_cmp(S, V, LB + 1) >= 0) {
                b_sub(S, V, LB + 1);
                q[bit >> 5] |= (limb)1 << (bit & 31);
            }
        }
    }
    b_muls(ln2, jexp, LG);
    b_sub(q, ln2, LG); /* q = gamma */

    /* E = exp(1 - gamma) */
    limb *x = (limb *)calloc(LG, sizeof(limb));
    x[FG] = 1; b_sub(x, q, LG);
    limb *xr = (limb *)calloc(LG, sizeof(limb));
    for (int i = 0; i + 1 < LG; i++) xr[i] = x[i + 1];
    limb *E = (limb *)calloc(LG, sizeof(limb));
    memset(t, 0, LG * sizeof(limb));
    E[FG] = 1; t[FG] = 1;
    for (uint32_t k = 1;; k++) {
        b_mul(t, LG, t, LG, xr, LG, FG);
        b_divs(t, k, LG);
        if (b_zero(t, LG)) break;
        b_add(E, t, LG);
    }
    for (int i = 0; i < 32; i++) b_mul(E, LG, E, LG, E, LG, FG);

    /* e */
    limb *ee = (limb *)calloc(LG, sizeof(limb));
    memset(t, 0, LG * sizeof(limb));
    ee[FG] = 1; t[FG] = 1;
    for (uint32_t k = 1;; k++) {
        b_divs(t, k, LG);
        if (b_zero(t, LG)) break;
        b_add(ee, t, LG);
    }

    limb *X = (limb *)calloc(LX, sizeof(limb));
    limb *ec = (limb *)calloc(FC + 1, sizeof(limb));
    for (int i = 0; i < FC + 1; i++) { X[i] = E[i + 2]; ec[i] = ee[i + 2]; }

    for (int m = 1; m <= Mmax; m++) {
        if (m > 1) b_mul(X, LX, X, LX, ec, FC + 1, FC);
        if (m <= lastm) continue;
        uint64_t fx = ((uint64_t)X[FC - 1] << 32) | X[FC - 2];
        long double fr = (long double)fx / 18446744073709551616.0L;
        long double c = (m < 80) ? expl(GAM - (long double)m) / 24.0L : 0.0L;
        long double tt = fr - 0.5L - c;
        int up; long double d;
        if (tt > 0) { up = 1; d = 1.0L - tt; } else { up = 0; d = -tt; }
        if (d <= 0) d = 1e-30L;
        long double lg = logl(d) - ((long double)m - GAM);
        if (lg < minlog) {
            seq_t v = 0;
            int started = 0;
            for (int i = LX - 1; i >= FC; i--) {
                if (!started && X[i] == 0) continue;
                started = 1;
                v = ((v << 16) << 16) + (idx_t)X[i];
            }
            if (up) v += 1;
            rec(lg, v);
        }
        lastm = m;
    }

    free(p); free(ln2); free(t); free(B); free(C); free(S); free(V); free(q);
    free(x); free(xr); free(E); free(ee); free(X); free(ec);
}

seq_t a(idx_t n) {
    if (!inited) { direct(); inited = 1; }
    while (cnt < n) {
        int M = 2 * lastm;
        int M2 = (int)(1.3 * (double)n) + 40;
        if (M2 > M) M = M2;
        extend(M);
    }
    return res[n - 1];
}
