/*
 * A004193  a(n) = -(-1)^n*2*(2*n+1)!*Bernoulli(2*n)/(n!*2^n).
 * https://oeis.org/A004193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   a(n) = 2*(2n+1)!!*|Bernoulli(2n)| = (2n+1)!! * n * T(n) /
 *            (2^(2n-2) * (4^n - 1)), where T(n) = A000182(n) are the tangent
 *            numbers (1, 2, 16, 272, ...), computed by the Brent-Harvey
 *            recurrence: T(1) = 1, T(k) = (k-1)*T(k-1), then for k = 2..N, j
 *            = k..N: T(j) = (j-k)*T(j-1) + (j-k+2)*T(j). Exact mu...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t len, cap; } bn_t;

static void bn_reserve(bn_t *x, size_t c) {
    if (x->cap < c) {
        size_t nc = x->cap * 2;
        if (nc < c) nc = c;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void bn_norm(bn_t *x) {
    while (x->len > 0 && x->d[x->len - 1] == 0) x->len--;
}

/* y = a*x + b*y  (x and y distinct) */
static void bn_muladd(bn_t *y, const bn_t *x, uint32_t a, uint32_t b) {
    size_t L = x->len > y->len ? x->len : y->len;
    size_t i;
    unsigned __int128 c = 0;
    bn_reserve(y, L + 1);
    for (i = 0; i < L; i++) {
        uint64_t xi = i < x->len ? x->d[i] : 0;
        uint64_t yi = i < y->len ? y->d[i] : 0;
        c += (unsigned __int128)a * xi + (unsigned __int128)b * yi;
        y->d[i] = (uint32_t)c;
        c >>= 32;
    }
    y->d[L] = (uint32_t)c;
    y->len = L + 1;
    bn_norm(y);
}

static void bn_mul_small(bn_t *x, uint32_t m) {
    size_t i;
    uint64_t c = 0;
    for (i = 0; i < x->len; i++) {
        c += (uint64_t)x->d[i] * m;
        x->d[i] = (uint32_t)c;
        c >>= 32;
    }
    if (c) {
        bn_reserve(x, x->len + 1);
        x->d[x->len++] = (uint32_t)c;
    }
}

static void bn_shr(bn_t *x, size_t s) {
    size_t o = s / 32, b = s % 32, i;
    if (o >= x->len) { x->len = 0; return; }
    for (i = 0; i + o < x->len; i++) {
        uint32_t lo = x->d[i + o];
        uint32_t hi = (i + o + 1 < x->len) ? x->d[i + o + 1] : 0;
        x->d[i] = b ? (uint32_t)((lo >> b) | (hi << (32 - b))) : lo;
    }
    x->len -= o;
    bn_norm(x);
}

/* exact division of x by 2^m - 1, via 2-adic inverse -(1 + 2^m + 2^2m + ...) */
static void bn_div_pow2m1(bn_t *x, size_t m) {
    size_t L = x->len, K = 32 * L, sh, p;
    uint32_t *acc;
    uint64_t c;
    if (L == 0) return;
    acc = (uint32_t *)calloc(L, sizeof(uint32_t));
    for (sh = 0; sh < K; sh += m) {
        size_t o = sh / 32, b = sh % 32;
        c = 0;
        for (p = o; p < L; p++) {
            size_t i = p - o;
            uint32_t cur = x->d[i];
            uint32_t prev = i > 0 ? x->d[i - 1] : 0;
            uint32_t v = b ? (uint32_t)((cur << b) | (prev >> (32 - b))) : cur;
            c += (uint64_t)acc[p] + v;
            acc[p] = (uint32_t)c;
            c >>= 32;
        }
    }
    c = 1;
    for (p = 0; p < L; p++) {
        c += (uint64_t)(uint32_t)~acc[p];
        x->d[p] = (uint32_t)c;
        c >>= 32;
    }
    free(acc);
    bn_norm(x);
}

static bn_t *Ttab = NULL;
static idx_t TN = 0;

static void build_tangent(idx_t N) {
    idx_t k, j, i;
    if (Ttab) {
        for (i = 0; i <= TN; i++) free(Ttab[i].d);
        free(Ttab);
    }
    Ttab = (bn_t *)calloc((size_t)N + 1, sizeof(bn_t));
    bn_reserve(&Ttab[1], 1);
    Ttab[1].d[0] = 1;
    Ttab[1].len = 1;
    for (k = 2; k <= N; k++)
        bn_muladd(&Ttab[k], &Ttab[k - 1], (uint32_t)(k - 1), 0);
    for (k = 2; k <= N; k++)
        for (j = k; j <= N; j++)
            bn_muladd(&Ttab[j], &Ttab[j - 1], (uint32_t)(j - k), (uint32_t)(j - k + 2));
    TN = N;
}

seq_t a(idx_t n) {
    bn_t X;
    uint64_t m;
    idx_t k;
    size_t i;
    seq_t r = 0;
    if (n < 1) return 0;
    if (n > TN) {
        idx_t N = n;
        if (N < 2 * TN) N = 2 * TN;
        if (N < 32) N = 32;
        build_tangent(N);
    }
    X.d = NULL; X.len = 0; X.cap = 0;
    bn_muladd(&X, &Ttab[n], (uint32_t)n, 0);      /* X = n * T(n) */
    m = 1;
    for (k = 3; k <= 2 * n + 1; k += 2) {           /* X *= (2n+1)!! */
        if (m * k > 0xFFFFFFFFULL) { bn_mul_small(&X, (uint32_t)m); m = 1; }
        m *= k;
    }
    if (m > 1) bn_mul_small(&X, (uint32_t)m);
    bn_shr(&X, (size_t)(2 * n - 2));
    bn_div_pow2m1(&X, (size_t)(2 * n));
    for (i = X.len; i > 0; i--) {
        uint32_t limb = X.d[i - 1];
        r = r * 256; r = r * 256;
        r = r + (seq_t)(idx_t)(limb >> 16);
        r = r * 256; r = r * 256;
        r = r + (seq_t)(idx_t)(limb & 0xFFFFu);
    }
    free(X.d);
    return r;
}
