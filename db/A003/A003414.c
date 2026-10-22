/*
 * A003414  a(n) = floor( Bernoulli(2*n)/(-4*n) ).
 * https://oeis.org/A003414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor( Bernoulli(2*n)/(-4*n) ) = floor( (-1)^n * T(n) /
 *            (2^(2n+1)*(4^n-1)) ), where T(n) = A000182(n) are the tangent
 *            numbers, obtained exactly as the zigzag numbers E(2n-1) from the
 *            Seidel-Entringer (boustrophedon) triangle: E(m,0) = 0, E(m,k) =
 *            E(m,k-1) + E(m-1,m-k), E(0,0) = 1
 * Range:     i8 n <= 10, i16 n <= 13, i32 n <= 16, i64 n <= 22, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t *d;
    size_t len;
    size_t cap;
} x3414_big;

static x3414_big *x3414_row = NULL;
static x3414_big *x3414_nrow = NULL;
static size_t x3414_rowcap = 0;
static idx_t x3414_m = 0;

static x3414_big *x3414_tan = NULL;
static size_t x3414_tancap = 0;
static idx_t x3414_th = 0;

static void x3414_reserve(x3414_big *b, size_t c) {
    if (b->cap < c) {
        size_t nc = c * 2 + 4;
        b->d = (uint32_t *)realloc(b->d, nc * sizeof(uint32_t));
        b->cap = nc;
    }
}

static void x3414_norm(x3414_big *b) {
    while (b->len > 0 && b->d[b->len - 1] == 0) b->len--;
}

static void x3414_copy(x3414_big *dst, const x3414_big *src) {
    x3414_reserve(dst, src->len + 1);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

/* dst = a + b; dst may alias a or b */
static void x3414_add(x3414_big *dst, const x3414_big *a, const x3414_big *b) {
    size_t la = a->len, lb = b->len;
    size_t n = (la > lb ? la : lb) + 1;
    size_t i;
    uint64_t c = 0;
    x3414_reserve(dst, n);
    for (i = 0; i + 1 < n; i++) {
        c += (uint64_t)(i < la ? a->d[i] : 0) + (uint64_t)(i < lb ? b->d[i] : 0);
        dst->d[i] = (uint32_t)c;
        c >>= 32;
    }
    dst->d[n - 1] = (uint32_t)c;
    dst->len = n;
    x3414_norm(dst);
}

static size_t x3414_bitlen(const x3414_big *b) {
    uint32_t t;
    size_t r;
    if (b->len == 0) return 0;
    t = b->d[b->len - 1];
    r = (b->len - 1) * 32;
    while (t) { r++; t >>= 1; }
    return r;
}

/* dst = src >> s; dst distinct from src */
static void x3414_shr(x3414_big *dst, const x3414_big *src, size_t s) {
    size_t ls = s / 32, bs = s % 32, n, i;
    if (src->len <= ls) { dst->len = 0; return; }
    n = src->len - ls;
    x3414_reserve(dst, n);
    for (i = 0; i < n; i++) {
        uint32_t v = src->d[i + ls] >> bs;
        if (bs && i + ls + 1 < src->len) v |= src->d[i + ls + 1] << (32 - bs);
        dst->d[i] = v;
    }
    dst->len = n;
    x3414_norm(dst);
}

/* dst = src mod 2^s; dst distinct from src */
static void x3414_low(x3414_big *dst, const x3414_big *src, size_t s) {
    size_t ls = s / 32, bs = s % 32;
    size_t n = ls + (bs ? 1 : 0);
    if (n > src->len) n = src->len;
    x3414_reserve(dst, n + 1);
    if (n) memcpy(dst->d, src->d, n * sizeof(uint32_t));
    if (bs && n == ls + 1) dst->d[ls] &= (((uint32_t)1 << bs) - 1);
    dst->len = n;
    x3414_norm(dst);
}

/* true if src mod 2^s == 0 */
static bool x3414_lowzero(const x3414_big *src, size_t s) {
    size_t ls = s / 32, bs = s % 32, i;
    for (i = 0; i < ls && i < src->len; i++)
        if (src->d[i]) return false;
    if (bs && ls < src->len && (src->d[ls] & (((uint32_t)1 << bs) - 1))) return false;
    return true;
}

/* true if b == 2^k - 1 */
static bool x3414_allones(const x3414_big *b, size_t k) {
    size_t i;
    if (x3414_bitlen(b) != k) return false;
    for (i = 0; i < k / 32; i++)
        if (b->d[i] != 0xFFFFFFFFu) return false;
    if (k % 32 && b->d[k / 32] != (((uint32_t)1 << (k % 32)) - 1)) return false;
    return true;
}

static void x3414_ensure_rows(size_t need) {
    size_t nc, i;
    if (need <= x3414_rowcap) return;
    nc = need * 2;
    x3414_row = (x3414_big *)realloc(x3414_row, nc * sizeof(x3414_big));
    x3414_nrow = (x3414_big *)realloc(x3414_nrow, nc * sizeof(x3414_big));
    for (i = x3414_rowcap; i < nc; i++) {
        x3414_row[i].d = NULL; x3414_row[i].len = 0; x3414_row[i].cap = 0;
        x3414_nrow[i].d = NULL; x3414_nrow[i].len = 0; x3414_nrow[i].cap = 0;
    }
    x3414_rowcap = nc;
}

static void x3414_ensure_tan(size_t need) {
    size_t nc, i;
    if (need <= x3414_tancap) return;
    nc = need * 2;
    x3414_tan = (x3414_big *)realloc(x3414_tan, nc * sizeof(x3414_big));
    for (i = x3414_tancap; i < nc; i++) {
        x3414_tan[i].d = NULL; x3414_tan[i].len = 0; x3414_tan[i].cap = 0;
    }
    x3414_tancap = nc;
}

static void x3414_step(void) {
    idx_t m = x3414_m + 1;
    idx_t k;
    x3414_big *t;
    x3414_ensure_rows((size_t)m + 1);
    x3414_nrow[0].len = 0;
    for (k = 1; k <= m; k++)
        x3414_add(&x3414_nrow[k], &x3414_nrow[k - 1], &x3414_row[m - k]);
    t = x3414_row; x3414_row = x3414_nrow; x3414_nrow = t;
    x3414_m = m;
    if (m & 1) {
        idx_t ti = (m + 1) / 2;
        x3414_ensure_tan((size_t)ti + 1);
        x3414_copy(&x3414_tan[ti], &x3414_row[m]);
        x3414_th = ti;
    }
}

seq_t a(idx_t n) {
    x3414_big y, h, l, q, one;
    size_t k, s;
    bool nz;
    seq_t r;
    size_t i;
    uint32_t onev = 1;

    if (n < 1) return 0;
    if (x3414_row == NULL) {
        x3414_ensure_rows(4);
        x3414_reserve(&x3414_row[0], 1);
        x3414_row[0].d[0] = 1;
        x3414_row[0].len = 1;
        x3414_m = 0;
        x3414_th = 0;
    }
    while (x3414_th < n) x3414_step();

    y.d = NULL; y.len = 0; y.cap = 0;
    h = y; l = y; q = y;
    one.d = &onev; one.len = 1; one.cap = 1;

    s = (size_t)(2 * n + 1);
    k = (size_t)(2 * n);
    x3414_shr(&y, &x3414_tan[n], s);
    nz = !x3414_lowzero(&x3414_tan[n], s);

    /* divide y by 2^k - 1 */
    while (x3414_bitlen(&y) > k) {
        x3414_shr(&h, &y, k);
        x3414_low(&l, &y, k);
        x3414_add(&q, &q, &h);
        x3414_add(&y, &h, &l);
    }
    if (x3414_allones(&y, k)) {
        x3414_add(&q, &q, &one);
        y.len = 0;
    }
    if (y.len != 0) nz = true;

    r = 0;
    i = q.len;
    while (i-- > 0) {
        r = r * 65536 + (idx_t)(q.d[i] >> 16);
        r = r * 65536 + (idx_t)(q.d[i] & 0xFFFFu);
    }
    if (n & 1) {
        r = -r;
        if (nz) r = r - 1;
    }

    free(y.d); free(h.d); free(l.d); free(q.d);
    return r;
}
