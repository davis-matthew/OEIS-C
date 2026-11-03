/*
 * A002882  Nearest integer to Bernoulli number B_{2n}.
 * https://oeis.org/A002882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = round(B_{2n}), with B_{2n} = (-1)^(n-1) * 2n * T(n) /
 *            (4^n*(4^n-1)) for n >= 1, where T(n) = A000182(n) (tangent
 *            numbers), computed by the Brent-Harvey recurrence: T(1) = 1,
 *            T(k) = (k-1)*T(k-1) initially; then for k = 2..N, j = k..N: T(j)
 *            = (j-k)*T(j-1) + (j-k+2)*T(j). a(0) = 1
 * Range:     i8 n <= 9, i16 n <= 11, i32 n <= 15, i64 n <= 21, i128 n <= 31,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 171 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void bl_add(uint32_t *a, const uint32_t *b, size_t W) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < W; i++) {
        uint64_t v = (uint64_t)a[i] + b[i] + carry;
        a[i] = (uint32_t)v;
        carry = v >> 32;
    }
}

static void bl_addbit(uint32_t *x, size_t W, uint64_t pos) {
    size_t idx = (size_t)(pos / 32);
    uint64_t v = (uint64_t)x[idx] + ((uint64_t)1 << (pos % 32));
    x[idx] = (uint32_t)v;
    uint64_t carry = v >> 32;
    idx++;
    while (carry && idx < W) {
        v = (uint64_t)x[idx] + carry;
        x[idx] = (uint32_t)v;
        carry = v >> 32;
        idx++;
    }
}

static int bl_allones(const uint32_t *x, uint64_t k) {
    size_t full = (size_t)(k / 32), i;
    unsigned bs = (unsigned)(k % 32);
    for (i = 0; i < full; i++)
        if (x[i] != 0xFFFFFFFFu) return 0;
    if (bs) {
        uint32_t m = ((uint32_t)1 << bs) - 1;
        if ((x[full] & m) != m) return 0;
    }
    return 1;
}

static size_t bl_bits(const uint32_t *x, size_t W) {
    size_t i = W;
    while (i > 0 && x[i - 1] == 0) i--;
    if (i == 0) return 0;
    uint32_t v = x[i - 1];
    size_t b = 0;
    while (v) { b++; v >>= 1; }
    return (i - 1) * 32 + b;
}

static void bl_mask(uint32_t *x, size_t W, uint64_t k) {
    size_t ls = (size_t)(k / 32), i;
    unsigned bs = (unsigned)(k % 32);
    if (ls < W) x[ls] &= bs ? (((uint32_t)1 << bs) - 1) : 0;
    for (i = ls + 1; i < W; i++) x[i] = 0;
}

static void bl_shr(uint32_t *dst, const uint32_t *src, size_t W, uint64_t s) {
    size_t ls = (size_t)(s / 32), i;
    unsigned bs = (unsigned)(s % 32);
    for (i = 0; i < W; i++) {
        size_t si = i + ls;
        uint32_t lo = si < W ? src[si] : 0;
        uint32_t hi = si + 1 < W ? src[si + 1] : 0;
        dst[i] = bs ? ((lo >> bs) | (hi << (32 - bs))) : lo;
    }
}

static void bl_subbit(uint32_t *x, size_t W, uint64_t pos) {
    size_t idx = (size_t)(pos / 32);
    uint32_t b = (uint32_t)1 << (pos % 32);
    if (x[idx] >= b) {
        x[idx] -= b;
        return;
    }
    x[idx] -= b;
    idx++;
    while (idx < W && x[idx] == 0) {
        x[idx] = 0xFFFFFFFFu;
        idx++;
    }
    if (idx < W) x[idx]--;
}

static unsigned t_clg2(uint64_t x) {
    unsigned b = 0;
    while (((uint64_t)1 << b) < x) b++;
    return b;
}

static size_t t_len(idx_t j) {
    return (size_t)((2 * j * t_clg2(2 * j) + 64) / 32 + 2);
}

static uint32_t *t_tab = NULL;
static size_t *t_off = NULL;
static idx_t t_N = 0;

static void t_build(idx_t N) {
    size_t *off = (size_t *)malloc((size_t)(N + 2) * sizeof(size_t));
    size_t tot = 0;
    idx_t j, k;
    off[0] = 0;
    for (j = 1; j <= N; j++) {
        off[j] = tot;
        tot += t_len(j);
    }
    off[N + 1] = tot;
    uint32_t *T = (uint32_t *)calloc(tot + 1, sizeof(uint32_t));
    T[off[1]] = 1;
    for (k = 2; k <= N; k++) {
        uint32_t *p = T + off[k - 1], *c = T + off[k];
        size_t lp = t_len(k - 1), i;
        uint64_t carry = 0;
        for (i = 0; i < lp; i++) {
            uint64_t v = (uint64_t)(k - 1) * p[i] + carry;
            c[i] = (uint32_t)v;
            carry = v >> 32;
        }
        c[lp] = (uint32_t)carry;
    }
    for (k = 2; k <= N; k++) {
        for (j = k; j <= N; j++) {
            uint64_t m1 = (uint64_t)(j - k), m2 = (uint64_t)(j - k + 2);
            uint32_t *p = T + off[j - 1], *c = T + off[j];
            size_t len = t_len(j), lp = t_len(j - 1), i;
            uint64_t carry = 0;
            for (i = 0; i < len; i++) {
                uint64_t pv = i < lp ? p[i] : 0;
                uint64_t v = m1 * pv + m2 * c[i] + carry;
                c[i] = (uint32_t)v;
                carry = v >> 32;
            }
        }
    }
    free(t_tab);
    free(t_off);
    t_tab = T;
    t_off = off;
    t_N = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n > t_N) {
        idx_t nn = t_N + t_N / 2;
        if (nn < n) nn = n;
        if (nn < 32) nn = 32;
        t_build(nn);
    }
    const uint32_t *Tn = t_tab + t_off[n];
    size_t ln = t_len(n), i;
    size_t W = ln + 3;
    size_t w2 = (size_t)(4 * n / 32) + 4;
    if (w2 > W) W = w2;

    uint32_t *Y = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *X = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *h = (uint32_t *)calloc(W, sizeof(uint32_t));

    /* Y = 4n*T + 2^(4n) - 2^(2n) */
    uint64_t carry = 0;
    for (i = 0; i < ln; i++) {
        uint64_t v = (uint64_t)(4 * n) * Tn[i] + carry;
        Y[i] = (uint32_t)v;
        carry = v >> 32;
    }
    for (i = ln; i < W && carry; i++) {
        Y[i] = (uint32_t)carry;
        carry >>= 32;
    }
    bl_addbit(Y, W, 4 * n);
    bl_subbit(Y, W, 2 * n);

    /* X = floor(Y / 2^(2n+1)); q = floor(X / (2^(2n) - 1)) */
    bl_shr(X, Y, W, 2 * n + 1);
    uint64_t k = 2 * n;
    while (bl_bits(X, W) > k) {
        bl_shr(h, X, W, k);
        bl_mask(X, W, k);
        bl_add(q, h, W);
        bl_add(X, h, W);
    }
    if (bl_allones(X, k)) bl_addbit(q, W, 0);

    seq_t r = 0;
    int started = 0;
    for (i = W; i-- > 0;) {
        if (!started && q[i] == 0) continue;
        started = 1;
        r = r * 65536 + (seq_t)(long long)(q[i] >> 16);
        r = r * 65536 + (seq_t)(long long)(q[i] & 0xFFFFu);
    }
    free(Y);
    free(X);
    free(q);
    free(h);
    if (n % 2 == 0) r = -r;
    return r;
}
