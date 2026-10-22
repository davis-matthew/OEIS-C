/*
 * A004114  Number of trees with n nodes and 2-colored internal (non-leaf)
 *          nodes.
 * https://oeis.org/A004114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: 1 + B(x) - x*B(x) - B(x)^2/2 + B(x^2)/2, B(x) = g.f. of
 *            A004113 (rooted trees with 2-colored internal nodes): B(x) = x +
 *            2x*(E(x) - 1), E(x) = exp(Sum_{k>=1} B(x^k)/k), computed as
 *            n*e(n) = Sum_{k=1..n} c(k)*e(n-k), c(k) = Sum_{d|k} d*B(d), e(0)
 *            = 1, B(1) = 1, B(m) = 2*e(m-1) for m >= 2. Th...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 36, u128 n <= 66,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/grow_zero.h"

/* ---- fixed-width path: exact 256-bit arithmetic (all terms fitting in 128 bits are safe) ---- */
#define FL 8
#define FMAX 120
typedef struct { uint32_t w[FL]; } u256;

static void u_set(u256 *r, uint32_t v) { int i; for (i = 0; i < FL; i++) r->w[i] = 0; r->w[0] = v; }
static void u_add(u256 *r, const u256 *a, const u256 *b) {
    uint64_t c = 0; int i;
    for (i = 0; i < FL; i++) { c += (uint64_t)a->w[i] + b->w[i]; r->w[i] = (uint32_t)c; c >>= 32; }
}
static void u_sub(u256 *r, const u256 *a, const u256 *b) {
    int64_t br = 0; int i;
    for (i = 0; i < FL; i++) {
        int64_t t = (int64_t)a->w[i] - (int64_t)b->w[i] - br;
        br = t < 0; r->w[i] = (uint32_t)t;
    }
}
static void u_mul(u256 *r, const u256 *a, const u256 *b) {
    uint32_t res[FL]; int i, j;
    for (i = 0; i < FL; i++) res[i] = 0;
    for (i = 0; i < FL; i++) {
        uint64_t carry = 0;
        for (j = 0; i + j < FL; j++) {
            uint64_t t = (uint64_t)a->w[i] * b->w[j] + res[i + j] + carry;
            res[i + j] = (uint32_t)t; carry = t >> 32;
        }
    }
    for (i = 0; i < FL; i++) r->w[i] = res[i];
}
static void u_divsmall(u256 *a, uint32_t d) {
    uint64_t rem = 0; int i;
    for (i = FL - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | a->w[i];
        a->w[i] = (uint32_t)(cur / d); rem = cur % d;
    }
}

static u256 FB[FMAX + 1], FE[FMAX + 1], FC[FMAX + 1];
static int fhave = 0;

static void f_extend(int N) {
    int m, k;
    if (fhave == 0) { u_set(&FB[0], 0); u_set(&FE[0], 1); u_set(&FC[0], 0); fhave = 1; }
    for (m = fhave; m <= N; m++) {
        u256 t, s;
        if (m == 1) u_set(&FB[1], 1);
        else { u_add(&FB[m], &FE[m - 1], &FE[m - 1]); }
        u_set(&FC[m], 0);
        for (k = 1; k <= m; k++) if (m % k == 0) {
            u_set(&t, (uint32_t)k); u_mul(&t, &t, &FB[k]); u_add(&FC[m], &FC[m], &t);
        }
        u_set(&s, 0);
        for (k = 1; k <= m; k++) { u_mul(&t, &FC[k], &FE[m - k]); u_add(&s, &s, &t); }
        u_divsmall(&s, (uint32_t)m);
        FE[m] = s;
    }
    if (N + 1 > fhave) fhave = N + 1;
}

static seq_t a_fixed(idx_t n) {
    int nn, i;
    u256 t, s, r;
    unsigned __int128 v = 0;
    if (n == 0) return 1;
    if (n > FMAX) return 0;
    nn = (int)n;
    f_extend(nn);
    u_set(&s, 0);
    for (i = 1; i < nn; i++) { u_mul(&t, &FB[i], &FB[nn - i]); u_add(&s, &s, &t); }
    if (nn % 2 == 0) u_sub(&s, &s, &FB[nn / 2]);
    u_divsmall(&s, 2);
    u_sub(&r, &FB[nn], &FB[nn - 1]);
    u_sub(&r, &r, &s);
    for (i = 3; i >= 0; i--) v = (v << 32) | r.w[i];
    return u128_to_seq(v);
}

/* ---- arbitrary-precision path ---- */
static seq_t *GB = 0, *GE = 0, *GC = 0;
static idx_t ghave = 0, gcap = 0;

static void g_extend(idx_t N) {
    idx_t m, k;
    if (N + 1 > gcap) {
        idx_t nc = gcap ? gcap : 64;
        while (nc < N + 1) nc *= 2;
        GB = grow_zero(GB, gcap, nc);
        GE = grow_zero(GE, gcap, nc);
        GC = grow_zero(GC, gcap, nc);
        gcap = nc;
    }
    if (ghave == 0) { GB[0] = 0; GE[0] = 1; GC[0] = 0; ghave = 1; }
    for (m = ghave; m <= N; m++) {
        seq_t s = 0;
        if (m == 1) GB[1] = 1;
        else GB[m] = GE[m - 1] * 2;
        GC[m] = 0;
        for (k = 1; k <= m; k++) if (m % k == 0) GC[m] += GB[k] * (seq_t)k;
        for (k = 1; k <= m; k++) s += GC[k] * GE[m - k];
        GE[m] = s / (seq_t)m;
    }
    if (N + 1 > ghave) ghave = N + 1;
}

static seq_t a_big(idx_t n) {
    idx_t i;
    seq_t s = 0, r;
    if (n == 0) return 1;
    g_extend(n);
    for (i = 1; i < n; i++) s += GB[i] * GB[n - i];
    if (n % 2 == 0) s -= GB[n / 2];
    s /= 2;
    r = GB[n] - GB[n - 1];
    r -= s;
    return r;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed(n);
    return a_big(n);
}
