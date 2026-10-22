/*
 * A002177  Numerators of Cotesian numbers (not in lowest terms):
 *          A002176(n)*C(n,0).
 * https://oeis.org/A002177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A002176(n)*C(n,0), where the Cotesian numbers are C(n,k)
 *            = (-1)^(n-k)/(n*k!*(n-k)!) * Integral_{t=0..n} Product_{j=0..n,
 *            j<>k} (t-j) dt, and A002176(n) is the lcm of the reduced
 *            denominators of C(n,k), k=0..n. Computed exactly with internal
 *            multiprecision integers
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   c177_vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/c177_vfact.h"

typedef struct { uint32_t *d; size_t len; size_t cap; } c177_bn;

static void c177_init(c177_bn *b) { b->d = NULL; b->len = 0; b->cap = 0; }
static void c177_free(c177_bn *b) { free(b->d); b->d = NULL; b->len = b->cap = 0; }
static void c177_reserve(c177_bn *b, size_t c)
{
    if (c > b->cap) {
        size_t nc = b->cap * 2;
        if (nc < c) nc = c;
        b->d = (uint32_t *)realloc(b->d, nc * sizeof(uint32_t));
        b->cap = nc;
    }
}
static void c177_setu(c177_bn *b, uint32_t v)
{
    b->len = 0;
    if (v) { c177_reserve(b, 1); b->d[0] = v; b->len = 1; }
}
static void c177_copy(c177_bn *dst, const c177_bn *src)
{
    c177_reserve(dst, src->len);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}
static void c177_muls(c177_bn *b, uint32_t m)
{
    uint64_t carry = 0;
    size_t i;
    if (m == 0 || b->len == 0) { b->len = 0; return; }
    for (i = 0; i < b->len; i++) {
        uint64_t t = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) { c177_reserve(b, b->len + 1); b->d[b->len++] = (uint32_t)carry; }
}
static uint32_t c177_divs(c177_bn *b, uint32_t m)
{
    uint64_t rem = 0;
    size_t i = b->len;
    while (i-- > 0) {
        uint64_t cur = (rem << 32) | b->d[i];
        b->d[i] = (uint32_t)(cur / m);
        rem = cur % m;
    }
    while (b->len && b->d[b->len - 1] == 0) b->len--;
    return (uint32_t)rem;
}
static uint32_t c177_mods(const c177_bn *b, uint32_t m)
{
    uint64_t rem = 0;
    size_t i = b->len;
    while (i-- > 0) rem = ((rem << 32) | b->d[i]) % m;
    return (uint32_t)rem;
}
static void c177_add(c177_bn *a, const c177_bn *b)
{
    size_t n = a->len > b->len ? a->len : b->len, i;
    uint64_t carry = 0;
    c177_reserve(a, n + 1);
    for (i = a->len; i < n; i++) a->d[i] = 0;
    for (i = 0; i < n; i++) {
        uint64_t t = (uint64_t)a->d[i] + (i < b->len ? b->d[i] : 0) + carry;
        a->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    a->len = n;
    if (carry) a->d[a->len++] = (uint32_t)carry;
}
/* a -= b, requires a >= b */
static void c177_sub(c177_bn *a, const c177_bn *b)
{
    size_t i;
    int64_t borrow = 0;
    for (i = 0; i < a->len; i++) {
        int64_t t = (int64_t)a->d[i] - (i < b->len ? (int64_t)b->d[i] : 0) - borrow;
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a->d[i] = (uint32_t)t;
    }
    while (a->len && a->d[a->len - 1] == 0) a->len--;
}
static int c177_cmp(const c177_bn *a, const c177_bn *b)
{
    size_t i;
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    i = a->len;
    while (i-- > 0)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}
static void c177_mul(c177_bn *out, const c177_bn *a, const c177_bn *b)
{
    size_t i, j;
    if (a->len == 0 || b->len == 0) { out->len = 0; return; }
    c177_reserve(out, a->len + b->len);
    memset(out->d, 0, (a->len + b->len) * sizeof(uint32_t));
    for (i = 0; i < a->len; i++) {
        uint64_t carry = 0;
        for (j = 0; j < b->len; j++) {
            uint64_t t = (uint64_t)a->d[i] * b->d[j] + out->d[i + j] + carry;
            out->d[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        out->d[i + b->len] = (uint32_t)carry;
    }
    out->len = a->len + b->len;
    while (out->len && out->d[out->len - 1] == 0) out->len--;
}

seq_t a(idx_t n)
{
    uint32_t N = (uint32_t)n, i, j, k, p;
    char *comp = (char *)calloc(N + 3, 1);
    int *vL = (int *)calloc(N + 3, sizeof(int));
    int *eD = (int *)calloc(N + 3, sizeof(int));
    c177_bn *pm = (c177_bn *)malloc((N + 2) * sizeof(c177_bn));
    c177_bn *r = (c177_bn *)malloc((N + 1) * sizeof(c177_bn));
    c177_bn L, cur, tmp, c, P, M, X0, T;
    uint32_t deg;
    seq_t res;
    size_t li;

    for (i = 2; i <= N + 1; i++)
        if (!comp[i])
            for (j = 2 * i; j <= N + 1; j += i) comp[j] = 1;

    /* magnitudes of coefficients of Product_{j=0..N} (t-j) */
    for (i = 0; i < N + 2; i++) c177_init(&pm[i]);
    c177_setu(&pm[0], 1);
    deg = 0;
    for (j = 0; j <= N; j++) {
        c177_setu(&pm[deg + 1], 0);
        for (i = deg + 1; i >= 1; i--) {
            c177_muls(&pm[i], j);
            c177_add(&pm[i], &pm[i - 1]);
        }
        c177_muls(&pm[0], j);
        deg++;
    }

    /* L = lcm(1..N+1), r[i] = L/(i+1) */
    c177_init(&L);
    c177_setu(&L, 1);
    for (p = 2; p <= N + 1; p++) {
        uint64_t q;
        if (comp[p]) continue;
        for (q = p; q <= (uint64_t)N + 1; q *= p) { vL[p]++; c177_muls(&L, p); }
    }
    for (i = 0; i <= N; i++) {
        c177_init(&r[i]);
        c177_copy(&r[i], &L);
        c177_divs(&r[i], i + 1);
    }

    c177_init(&cur); c177_init(&tmp); c177_init(&c);
    c177_init(&P); c177_init(&M); c177_init(&X0); c177_init(&T);

    for (k = 0; k <= N / 2; k++) {
        /* |L * Integral_0^N Q_k(t) dt| via Horner, Q_k = Product_{j<>k}(t-j) */
        c177_setu(&cur, 1);
        c177_setu(&P, 0);
        c177_setu(&M, 0);
        i = N;
        for (;;) {
            if (i < N) {
                c177_copy(&tmp, &cur);
                c177_muls(&tmp, k);
                c177_copy(&cur, &pm[i + 1]);
                c177_sub(&cur, &tmp);
            }
            c177_muls(&P, N);
            c177_muls(&M, N);
            c177_mul(&c, &cur, &r[i]);
            if (((N - i) & 1) == 0) c177_add(&P, &c); else c177_add(&M, &c);
            if (i == 0) break;
            i--;
        }
        c177_muls(&P, N);
        c177_muls(&M, N);
        if (c177_cmp(&P, &M) >= 0) c177_sub(&P, &M);
        else { c177_sub(&M, &P); c177_copy(&P, &M); }
        if (k == 0) c177_copy(&X0, &P);
        if (P.len == 0) continue;

        for (p = 2; p <= N + 1; p++) {
            uint64_t ek, cnt = 0, t;
            int dexp;
            if (comp[p]) continue;
            ek = (uint64_t)vL[p] + c177_vfact(k, p) + c177_vfact(N - k, p);
            t = N;
            while (t % p == 0) { ek++; t /= p; }
            if (ek == 0) continue;
            c177_copy(&T, &P);
            while (cnt < ek && c177_mods(&T, p) == 0) { c177_divs(&T, p); cnt++; }
            dexp = (int)(ek - cnt);
            if (dexp > eD[p]) eD[p] = dexp;
        }
    }

    /* a(n) = N_0 * D / (L * n * n!) */
    for (p = 2; p <= N + 1; p++) {
        int e;
        if (comp[p]) continue;
        for (e = 0; e < eD[p]; e++) c177_muls(&X0, p);
    }
    c177_divs(&X0, N);
    for (j = 2; j <= N; j++) c177_divs(&X0, j);
    for (p = 2; p <= N + 1; p++) {
        int e;
        if (comp[p]) continue;
        for (e = 0; e < vL[p]; e++) c177_divs(&X0, p);
    }

    res = 0;
    li = X0.len;
    while (li-- > 0) {
        int s;
        for (s = 24; s >= 0; s -= 8) {
            res = (seq_t)(res * 256);
            res += (seq_t)((X0.d[li] >> s) & 255u);
        }
    }

    for (i = 0; i < N + 2; i++) c177_free(&pm[i]);
    for (i = 0; i <= N; i++) c177_free(&r[i]);
    free(pm); free(r);
    c177_free(&L); c177_free(&cur); c177_free(&tmp); c177_free(&c);
    c177_free(&P); c177_free(&M); c177_free(&X0); c177_free(&T);
    free(comp); free(vL); free(eD);
    return res;
}
