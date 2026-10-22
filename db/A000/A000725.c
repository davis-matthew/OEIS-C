/*
 * A000725  Invertible Boolean functions of n variables.
 * https://oeis.org/A000725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma: a(n) = (1/(n!^2*2^n)) * Sum_{g in S_n, h in
 *            B_n, g and h of equal cycle type on {0,1}^n} Prod_d d^c_d *
 *            c_d!, where c_d = number of d-cycles on {0,1}^n, c_d =
 *            (1/d)*Sum_{e|d} mu(d/e)*fix(e), fix(e) = Prod_{positive
 *            k-cycles} 2^gcd(e,k) * Prod_{negative k-cycles} [e/gcd(e,k)
 *            even...
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   fixexp725, g725, mu725, next725
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/fixexp725.h"
#include "../utils/g725.h"
#include "../utils/mu725.h"
#include "../utils/next725.h"

typedef unsigned __int128 u128_725;

typedef struct { uint32_t *v; size_t len, cap; } bn725;

static void bn725_reserve(bn725 *a, size_t c) {
    if (c > a->cap) {
        size_t nc = a->cap * 2;
        if (nc < c) nc = c;
        a->v = (uint32_t *)realloc(a->v, nc * sizeof(uint32_t));
        a->cap = nc;
    }
}
static void bn725_trim(bn725 *a) { while (a->len > 1 && a->v[a->len - 1] == 0) a->len--; }
static void bn725_set(bn725 *a, uint32_t x) { bn725_reserve(a, 1); a->v[0] = x; a->len = 1; }
static void bn725_mul32(bn725 *a, uint32_t m) {
    uint64_t c = 0;
    for (size_t i = 0; i < a->len; i++) {
        uint64_t t = (uint64_t)a->v[i] * m + c;
        a->v[i] = (uint32_t)t; c = t >> 32;
    }
    if (c) { bn725_reserve(a, a->len + 1); a->v[a->len++] = (uint32_t)c; }
}
static void bn725_mulw(bn725 *a, u128_725 m) {
    uint32_t ml[4]; int k = 0;
    while (m) { ml[k++] = (uint32_t)m; m >>= 32; }
    if (k == 0) { bn725_set(a, 0); return; }
    size_t L = a->len;
    uint32_t *r = (uint32_t *)calloc(L + k, sizeof(uint32_t));
    for (int j = 0; j < k; j++) {
        uint64_t c = 0;
        for (size_t i = 0; i < L; i++) {
            uint64_t t = (uint64_t)a->v[i] * ml[j] + r[i + j] + c;
            r[i + j] = (uint32_t)t; c = t >> 32;
        }
        r[L + j] = (uint32_t)c;
    }
    free(a->v);
    a->v = r; a->len = L + k; a->cap = L + k;
    bn725_trim(a);
}
static void bn725_add(bn725 *a, const bn725 *b) {
    size_t m = a->len > b->len ? a->len : b->len;
    bn725_reserve(a, m + 1);
    for (size_t i = a->len; i < m + 1; i++) a->v[i] = 0;
    uint64_t c = 0;
    for (size_t i = 0; i < m; i++) {
        c += (uint64_t)a->v[i] + (i < b->len ? b->v[i] : 0);
        a->v[i] = (uint32_t)c; c >>= 32;
    }
    a->v[m] = (uint32_t)c; a->len = m + 1;
    bn725_trim(a);
}
static void bn725_div32(bn725 *a, uint32_t d) {
    uint64_t r = 0;
    for (size_t i = a->len; i-- > 0;) {
        r = (r << 32) | a->v[i];
        a->v[i] = (uint32_t)(r / d); r %= d;
    }
    bn725_trim(a);
}
static void push725(bn725 *z, uint64_t *acc, uint64_t x) {
    if (x <= 1) return;
    if (*acc * x > 0xFFFFFFFFULL) { bn725_mul32(z, (uint32_t)*acc); *acc = x; }
    else *acc *= x;
}

seq_t a(idx_t n) {
    int N = (int)n;
    u128_725 fact = 1;
    for (int i = 2; i <= N; i++) fact *= (u128_725)i;
    u128_725 bfact = fact << N;

    int ns = 0, scap = 0;
    long long *sL = NULL; u128_725 *sz = NULL, *sS = NULL; int **sfix = NULL;
    int *p = (int *)malloc((N + 2) * sizeof(int));
    int k;
    p[0] = N; k = 1;
    do {
        if (ns == scap) {
            scap = scap ? 2 * scap : 16;
            sL = (long long *)realloc(sL, scap * sizeof(long long));
            sz = (u128_725 *)realloc(sz, scap * sizeof(u128_725));
            sS = (u128_725 *)realloc(sS, scap * sizeof(u128_725));
            sfix = (int **)realloc(sfix, scap * sizeof(int *));
        }
        long long L = 1; u128_725 z = 1; int run = 0;
        for (int i = 0; i < k; i++) {
            L = L / g725(L, p[i]) * p[i];
            if (i > 0 && p[i] == p[i - 1]) run++; else run = 1;
            z *= (u128_725)p[i] * (u128_725)run;
        }
        int *fx = (int *)calloc((size_t)L + 1, sizeof(int));
        for (long long d = 1; d <= L; d++)
            if (L % d == 0) fx[d] = fixexp725(p, k, NULL, 0, d);
        sL[ns] = L; sz[ns] = z; sS[ns] = 0; sfix[ns] = fx; ns++;
    } while ((k = next725(p, k)) > 0);

    int *pa = (int *)malloc((N + 2) * sizeof(int));
    int *pb = (int *)malloc((N + 2) * sizeof(int));
    int *bf = NULL; long long bfcap = 0;
    for (int as = 0; as <= N; as++) {
        int bs = N - as;
        int ka;
        if (as > 0) { pa[0] = as; ka = 1; } else ka = 0;
        do {
            int kb;
            if (bs > 0) { pb[0] = bs; kb = 1; } else kb = 0;
            do {
                long long L = 1;
                for (int i = 0; i < ka; i++) L = L / g725(L, pa[i]) * pa[i];
                for (int i = 0; i < kb; i++) L = L / g725(L, 2LL * pb[i]) * (2LL * pb[i]);
                int computed = 0;
                u128_725 csize = 0;
                for (int s = 0; s < ns; s++) {
                    if (sL[s] != L) continue;
                    if (!computed) {
                        if (L + 1 > bfcap) {
                            bfcap = L + 1;
                            bf = (int *)realloc(bf, (size_t)bfcap * sizeof(int));
                        }
                        for (long long d = 1; d <= L; d++)
                            if (L % d == 0) bf[d] = fixexp725(pa, ka, pb, kb, d);
                        u128_725 z = 1; int run = 0;
                        for (int i = 0; i < ka; i++) {
                            if (i > 0 && pa[i] == pa[i - 1]) run++; else run = 1;
                            z *= (u128_725)(2 * pa[i]) * (u128_725)run;
                        }
                        run = 0;
                        for (int i = 0; i < kb; i++) {
                            if (i > 0 && pb[i] == pb[i - 1]) run++; else run = 1;
                            z *= (u128_725)(2 * pb[i]) * (u128_725)run;
                        }
                        csize = bfact / z;
                        computed = 1;
                    }
                    int ok = 1;
                    for (long long d = 1; d <= L && ok; d++)
                        if (L % d == 0 && bf[d] != sfix[s][d]) ok = 0;
                    if (ok) sS[s] += csize;
                }
            } while ((kb = next725(pb, kb)) > 0);
        } while ((ka = next725(pa, ka)) > 0);
    }

    bn725 tot, Z;
    tot.v = NULL; tot.len = 0; tot.cap = 0;
    Z.v = NULL; Z.len = 0; Z.cap = 0;
    bn725_set(&tot, 0);
    for (int s = 0; s < ns; s++) {
        if (sS[s] == 0) continue;
        long long L = sL[s];
        int *fx = sfix[s];
        bn725_set(&Z, 1);
        uint64_t acc = 1;
        for (long long d = 1; d <= L; d++) {
            if (L % d) continue;
            long long sum = 0;
            for (long long e = 1; e <= d; e++) {
                if (d % e) continue;
                int m = mu725(d / e);
                if (m == 0 || fx[e] < 0) continue;
                sum += (long long)m * (1LL << fx[e]);
            }
            long long c = sum / d;
            for (long long j = 1; j <= c; j++) {
                push725(&Z, &acc, (uint64_t)d);
                push725(&Z, &acc, (uint64_t)j);
            }
        }
        if (acc > 1) bn725_mul32(&Z, (uint32_t)acc);
        bn725_mulw(&Z, fact / sz[s]);
        bn725_mulw(&Z, sS[s]);
        bn725_add(&tot, &Z);
    }
    for (int i = 0; i < N; i++) bn725_div32(&tot, 2);
    for (int i = 2; i <= N; i++) { bn725_div32(&tot, (uint32_t)i); bn725_div32(&tot, (uint32_t)i); }

    seq_t res = 0;
    for (size_t i = tot.len; i-- > 0;) {
        res = (seq_t)(res * (seq_t)65536);
        res = (seq_t)(res * (seq_t)65536);
        res = (seq_t)(res + (seq_t)tot.v[i]);
    }

    for (int s = 0; s < ns; s++) free(sfix[s]);
    free(sfix); free(sL); free(sz); free(sS);
    free(p); free(pa); free(pb); free(bf);
    free(tot.v); free(Z.v);
    return res;
}
