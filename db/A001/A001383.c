/*
 * A001383  Number of n-node rooted trees of height at most 3.
 * https://oeis.org/A001383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; for n >= 1, a(n) = b(n-1) where b is the Euler
 *            transform of p(k-1), p = A000041: c(k) = Sum_{d|k} d*p(d-1),
 *            b(0) = 1, b(m) = (1/m) * Sum_{k=1..m} c(k)*b(m-k). G.f.:
 *            x*Product_{k>=1} (1 - x^k)^(-p(k-1))
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 43, u64 n <= 95, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
typedef std::vector<seq_t> a1383_vec;
static void a1383_vinit(a1383_vec *v) { (void)v; }
static void a1383_vresize(a1383_vec *v, idx_t n) { v->resize((size_t)n); }
static seq_t *a1383_vdata(a1383_vec *v) { return v->data(); }
static void a1383_vrelease(a1383_vec *v) { a1383_vec().swap(*v); }
#else
typedef struct { seq_t *d; idx_t n; } a1383_vec;
static void a1383_vinit(a1383_vec *v) { v->d = NULL; v->n = 0; }
static void a1383_vresize(a1383_vec *v, idx_t n) {
    idx_t i;
    v->d = (seq_t *)realloc(v->d, (size_t)n * sizeof(seq_t));
    for (i = v->n; i < n; i++) v->d[i] = 0;
    v->n = n;
}
static seq_t *a1383_vdata(a1383_vec *v) { return v->d; }
static void a1383_vrelease(a1383_vec *v) { free(v->d); v->d = NULL; v->n = 0; }
#endif

typedef unsigned __int128 a1383_u128;

static int a1383_big(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x *= 2;
    return x != 0;
}

static void a1383_parts_seq(seq_t *p, idx_t M) {
    idx_t i, j;
    for (i = 0; i < M; i++) {
        seq_t pos = 0, neg = 0;
        if (i == 0) { p[0] = 1; continue; }
        for (j = 1;; j++) {
            idx_t g1 = j * (3 * j - 1) / 2, g2 = j * (3 * j + 1) / 2;
            if (g1 > i) break;
            if (j & 1) pos += p[i - g1]; else neg += p[i - g1];
            if (g2 <= i) { if (j & 1) pos += p[i - g2]; else neg += p[i - g2]; }
        }
        p[i] = pos - neg;
    }
}

static void a1383_parts_u(a1383_u128 *p, idx_t M) {
    idx_t i, j;
    for (i = 0; i < M; i++) {
        a1383_u128 s = 0;
        if (i == 0) { p[0] = 1; continue; }
        for (j = 1;; j++) {
            idx_t g1 = j * (3 * j - 1) / 2, g2 = j * (3 * j + 1) / 2;
            if (g1 > i) break;
            if (j & 1) s += p[i - g1]; else s -= p[i - g1];
            if (g2 <= i) { if (j & 1) s += p[i - g2]; else s -= p[i - g2]; }
        }
        p[i] = s;
    }
}

static a1383_u128 a1383_inv(a1383_u128 q) {
    a1383_u128 x = q;
    int i;
    for (i = 0; i < 7; i++) x *= (a1383_u128)2 - q * x;
    return x;
}

static a1383_vec a1383_bb;
static int a1383_binit = 0;
static idx_t a1383_bcap = 0;

static void a1383_ensure_big(idx_t N) {
    idx_t M, i, k, d;
    a1383_vec pv, cv;
    seq_t *p, *c, *b;
    if (!a1383_binit) { a1383_vinit(&a1383_bb); a1383_binit = 1; }
    if (N <= a1383_bcap) return;
    M = a1383_bcap * 2;
    if (M < N) M = N;
    if (M < 64) M = 64;
    a1383_vinit(&pv);
    a1383_vinit(&cv);
    a1383_vresize(&pv, M);
    a1383_vresize(&cv, M);
    a1383_vresize(&a1383_bb, M);
    p = a1383_vdata(&pv);
    c = a1383_vdata(&cv);
    b = a1383_vdata(&a1383_bb);
    a1383_parts_seq(p, M);
    for (i = 0; i < M; i++) c[i] = 0;
    for (d = 1; d < M; d++) {
        seq_t t = p[d - 1] * d;
        for (k = d; k < M; k += d) c[k] += t;
    }
    for (i = a1383_bcap; i < M; i++) {
        seq_t s = 0;
        if (i == 0) { b[0] = 1; continue; }
        for (k = 1; k <= i; k++) s += c[k] * b[i - k];
        b[i] = s / i;
    }
    a1383_vrelease(&pv);
    a1383_vrelease(&cv);
    a1383_bcap = M;
}

static a1383_u128 *a1383_fb = NULL;
static idx_t a1383_fcap = 0;

static void a1383_ensure_fixed(idx_t N) {
    idx_t M, i, j, k;
    a1383_u128 *p, *C, *res;
    if (N <= a1383_fcap) return;
    M = N + N / 4;
    if (M < 64) M = 64;
    p = (a1383_u128 *)malloc(M * sizeof(a1383_u128));
    C = (a1383_u128 *)malloc((M + 1) * sizeof(a1383_u128));
    res = (a1383_u128 *)malloc(M * sizeof(a1383_u128));
    a1383_parts_u(p, M);
    for (i = 0; i < M; i++) res[i] = 0;
    res[0] = 1;
    for (k = 1; k < M; k++) {
        a1383_u128 m = p[k - 1], O = 1;
        long long E = 0;
        idx_t J = (M - 1) / k;
        for (j = 1; j <= J; j++) {
            a1383_u128 t = m + (a1383_u128)(j - 1);
            idx_t u = j;
            if (t == 0) {
                E += 1000;
            } else {
                while ((t & 1) == 0) { t >>= 1; E++; }
                O *= t;
            }
            while ((u & 1) == 0) { u >>= 1; E--; }
            O *= a1383_inv((a1383_u128)u);
            C[j] = (E >= 128) ? (a1383_u128)0 : (O << E);
        }
        for (i = M - 1; i >= k; i--) {
            a1383_u128 s = 0;
            for (j = 1; j * k <= i; j++) s += C[j] * res[i - j * k];
            res[i] += s;
        }
    }
    free(p);
    free(C);
    if (a1383_fb) free(a1383_fb);
    a1383_fb = res;
    a1383_fcap = M;
}

static seq_t a1383_from_u(a1383_u128 v) {
    seq_t S = (seq_t)4294967296ULL;
    seq_t lo = (seq_t)(uint64_t)v;
    seq_t hi = (seq_t)(uint64_t)(v >> 64);
    hi = hi * S;
    hi = hi * S;
    return hi + lo;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (a1383_big()) {
        a1383_ensure_big(n);
        return a1383_vdata(&a1383_bb)[n - 1];
    }
    a1383_ensure_fixed(n);
    return a1383_from_u(a1383_fb[n - 1]);
}
