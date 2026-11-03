/*
 * A006771  Exponential self-convolution of numbers of trees on n nodes.
 * https://oeis.org/A006771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*t(k)*t(n-k), where t = A000055
 *            (trees on n nodes), t(n) = r(n) - (Sum_{k=1..n-1} r(k)*r(n-k) -
 *            [n even]*r(n/2))/2, t(0) = 1, r = A000081: r(1) = 1, r(n+1) =
 *            (1/n)*Sum_{k=1..n} (Sum_{d|k} d*r(d))*r(n-k+1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 33, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 984 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

typedef unsigned __int128 u128_t;

/* ---- exact path in arbitrary precision (seq_t) ---- */
static seq_t *tb = NULL;
static idx_t tb_len = 0;

static void build_big(idx_t N) {
    idx_t i, k, m;
    seq_t *r = alloc_zero(N + 2);
    seq_t *s = alloc_zero(N + 2);
    seq_t *t = alloc_zero(N + 1);
    r[1] = 1;
    for (m = 1; m <= N; m++) s[m] += r[1];
    for (i = 1; i < N; i++) {
        seq_t sum = 0;
        for (k = 1; k <= i; k++) sum += s[k] * r[i - k + 1];
        r[i + 1] = sum / (seq_t)i;
        {
            seq_t c = (seq_t)(i + 1) * r[i + 1];
            for (m = i + 1; m <= N; m += i + 1) s[m] += c;
        }
    }
    t[0] = 1;
    for (i = 1; i <= N; i++) {
        seq_t c = 0;
        for (k = 1; k < i; k++) c += r[k] * r[i - k];
        if (i % 2 == 0) c -= r[i / 2];
        t[i] = r[i] - c / 2;
    }
    release(r, N + 2);
    release(s, N + 2);
    if (tb) release(tb, tb_len);
    tb = t;
    tb_len = N + 1;
}

/* ---- path for fixed-width builds, computed exactly in 128 bits ---- */
#define W_MAX 80
static u128_t tw[W_MAX + 1];
static bool tw_ready = false;

static void build_w(void) {
    idx_t i, k, m;
    idx_t N = W_MAX;
    u128_t r[W_MAX + 2], s[W_MAX + 2];
    for (i = 0; i <= N + 1; i++) { r[i] = 0; s[i] = 0; }
    r[1] = 1;
    for (m = 1; m <= N; m++) s[m] += r[1];
    for (i = 1; i < N; i++) {
        u128_t sum = 0;
        for (k = 1; k <= i; k++) sum += s[k] * r[i - k + 1];
        r[i + 1] = sum / i;
        {
            u128_t c = (u128_t)(i + 1) * r[i + 1];
            for (m = i + 1; m <= N; m += i + 1) s[m] += c;
        }
    }
    tw[0] = 1;
    for (i = 1; i <= N; i++) {
        u128_t c = 0;
        for (k = 1; k < i; k++) c += r[k] * r[i - k];
        if (i % 2 == 0) c -= r[i / 2];
        tw[i] = r[i] - c / 2;
    }
    tw_ready = true;
}

seq_t a(idx_t n) {
    idx_t k;
    if (seq_is_fixed_width()) {
        u128_t res = 0, c = 1;
        if (n > W_MAX) return 0; /* far beyond 128 bits; cannot fit any fixed width */
        if (!tw_ready) build_w();
        for (k = 0; k <= n; k++) {
            res += c * tw[k] * tw[n - k];
            if (k < n) c = c * (u128_t)(n - k) / (u128_t)(k + 1);
        }
        return u128_to_seq(res);
    } else {
        seq_t res = 0, c = 1;
        if (n >= tb_len) build_big(n < 16 ? 32 : n + n / 2);
        for (k = 0; k <= n; k++) {
            res += c * tb[k] * tb[n - k];
            if (k < n) c = c * (seq_t)(n - k) / (seq_t)(k + 1);
        }
        return res;
    }
}
