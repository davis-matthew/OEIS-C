/*
 * A001385  Number of n-node trees of height at most 5.
 * https://oeis.org/A001385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = R_5(n), where R_0(x) = x and R_h(x)
 *            = x * EULER(R_{h-1})(x), i.e. a = Euler transform of A001384
 *            shifted right. Euler transform: c(0) = 1, c(m) = (1/m) *
 *            Sum_{k=1..m} (Sum_{d|k} d*b(d)) * c(m-k)
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   a1385_euler_big, a1385_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a1385_euler_big.h"
#include "../utils/a1385_is_fixed.h"

typedef struct { uint64_t w[3]; } a1385_u192;

static a1385_u192 a1385_mk(uint64_t v) { a1385_u192 r = {{v, 0, 0}}; return r; }

static int a1385_iszero(a1385_u192 x) { return x.w[0] == 0 && x.w[1] == 0 && x.w[2] == 0; }

static a1385_u192 a1385_add(a1385_u192 x, a1385_u192 y) {
    a1385_u192 r;
    unsigned __int128 carry = 0;
    int i;
    for (i = 0; i < 3; i++) {
        unsigned __int128 t = (unsigned __int128)x.w[i] + y.w[i] + carry;
        r.w[i] = (uint64_t)t;
        carry = t >> 64;
    }
    return r;
}

static a1385_u192 a1385_mul(a1385_u192 x, a1385_u192 y) {
    uint64_t r[3] = {0, 0, 0};
    int i, j;
    for (i = 0; i < 3; i++) {
        unsigned __int128 carry = 0;
        for (j = 0; i + j < 3; j++) {
            unsigned __int128 t = (unsigned __int128)x.w[i] * y.w[j] + r[i + j] + carry;
            r[i + j] = (uint64_t)t;
            carry = t >> 64;
        }
    }
    a1385_u192 o = {{r[0], r[1], r[2]}};
    return o;
}

static a1385_u192 a1385_divs(a1385_u192 x, uint64_t d) {
    unsigned __int128 rem = 0;
    int i;
    for (i = 2; i >= 0; i--) {
        unsigned __int128 cur = (rem << 64) | x.w[i];
        x.w[i] = (uint64_t)(cur / d);
        rem = cur % d;
    }
    return x;
}

static void a1385_euler_fixed(const a1385_u192 *b, a1385_u192 *c, idx_t N) {
    a1385_u192 *ds = (a1385_u192 *)malloc((size_t)(N + 1) * sizeof(a1385_u192));
    idx_t d, k, m;
    for (k = 0; k <= N; k++) ds[k] = a1385_mk(0);
    for (d = 1; d <= N; d++) {
        if (a1385_iszero(b[d])) continue;
        a1385_u192 t = a1385_mul(a1385_mk(d), b[d]);
        for (k = d; k <= N; k += d) ds[k] = a1385_add(ds[k], t);
    }
    c[0] = a1385_mk(1);
    for (m = 1; m <= N; m++) {
        a1385_u192 s = a1385_mk(0);
        for (k = 1; k <= m; k++) s = a1385_add(s, a1385_mul(ds[k], c[m - k]));
        c[m] = a1385_divs(s, (uint64_t)m);
    }
    free(ds);
}

static a1385_u192 *a1385_fx = NULL;
static idx_t a1385_fxN = 0;
static seq_t *a1385_bg = NULL;
static idx_t a1385_bgN = 0;

static void a1385_build_fixed(idx_t N) {
    a1385_u192 *b = (a1385_u192 *)malloc((size_t)(N + 1) * sizeof(a1385_u192));
    a1385_u192 *c = (a1385_u192 *)malloc((size_t)(N + 1) * sizeof(a1385_u192));
    idx_t m;
    int h;
    for (m = 0; m <= N; m++) b[m] = a1385_mk(0);
    b[1] = a1385_mk(1);
    for (h = 1; h <= 5; h++) {
        a1385_euler_fixed(b, c, N);
        b[0] = a1385_mk(0);
        for (m = 1; m <= N; m++) b[m] = c[m - 1];
    }
    free(c);
    if (a1385_fx) free(a1385_fx);
    a1385_fx = b;
    a1385_fxN = N;
}

static void a1385_build_big(idx_t N) {
    seq_t *b = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *c = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    idx_t m;
    int h;
    for (m = 0; m <= N; m++) { b[m] = 0; c[m] = 0; }
    b[1] = 1;
    for (h = 1; h <= 5; h++) {
        a1385_euler_big(b, c, N);
        b[0] = 0;
        for (m = 1; m <= N; m++) b[m] = c[m - 1];
    }
    free(c);
    if (a1385_bg) free(a1385_bg);
    a1385_bg = b;
    a1385_bgN = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (a1385_is_fixed()) {
        /* terms beyond ~115 exceed 128 bits; internal 192-bit arithmetic is exact up to there */
        const idx_t CAP = 200;
        if (n > CAP) return 0;
        if (a1385_fx == NULL) a1385_build_fixed(CAP);
        a1385_u192 v = a1385_fx[n];
        seq_t r = (seq_t)v.w[2];
        int i;
        for (i = 0; i < 4; i++) r = r * 65536u;
        r += (seq_t)v.w[1];
        for (i = 0; i < 4; i++) r = r * 65536u;
        r += (seq_t)v.w[0];
        return r;
    }
    if (a1385_bg == NULL || n > a1385_bgN) {
        idx_t N = 64;
        if (a1385_bgN * 2 > N) N = a1385_bgN * 2;
        if (n > N) N = n;
        a1385_build_big(N);
    }
    return a1385_bg[n];
}
