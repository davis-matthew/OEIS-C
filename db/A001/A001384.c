/*
 * A001384  Number of n-node trees of height at most 4.
 * https://oeis.org/A001384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   rooted trees counted by height. R_0(x) = x, and for h >= 1
 *            R_h(x) = x * EulerT(R_{h-1})(x) = x * Prod_{k>=1}
 *            1/(1-x^k)^{r_{h-1}(k)}. a(n) = [x^n] R_4(x) for n >= 1, and a(0)
 *            = 1. Equivalently, a(n+1) is the Euler transform of A001383
 *            (shift right). Euler transform b of c: b(0) = 1, b(n) = (1/n)
 *            S...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 67, u128 n <= 139,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 177 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *a1384_zeros(idx_t len) {
    seq_t *p = mem_new(seq_t, len);
    idx_t i;
    for (i = 0; i < len; i++) p[i] = 0;
    return p;
}

/* b = Euler transform of c, indices 0..N, using the division recurrence */
static void a1384_euler_div(const seq_t *c, seq_t *b, idx_t N) {
    seq_t *s = a1384_zeros(N + 1);
    idx_t d, m, k;
    for (d = 1; d <= N; d++) {
        if (c[d] == 0) continue;
        seq_t t = (seq_t)d * c[d];
        for (m = d; m <= N; m += d) s[m] += t;
    }
    b[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t sum = 0;
        for (k = 1; k <= m; k++) sum += s[k] * b[m - k];
        b[m] = sum / (seq_t)m;
    }
    mem_free(s);
}

/* b = Euler transform of c, indices 0..N, by multiplying out the product */
static void a1384_euler_mc(const seq_t *c, seq_t *b, idx_t N) {
    seq_t *g = a1384_zeros(N + 1);
    idx_t k, j, m;
    b[0] = 1;
    for (m = 1; m <= N; m++) b[m] = 0;
    for (k = 1; k <= N; k++) {
        seq_t cc = c[k];
        if (cc == 0) continue;
        idx_t J = N / k;
        g[0] = 1;
        for (j = 1; j <= J; j++) {
            /* g[j] = g[j-1] * (cc + j - 1) / j, exactly, without overflow */
            seq_t jj = (seq_t)j;
            seq_t gg = gcd(g[j - 1], jj);
            seq_t t = jj / gg;
            seq_t q = (cc + (seq_t)(j - 1)) / t;
            g[j] = (g[j - 1] / gg) * q;
        }
        for (m = N; m >= k; m--) {
            seq_t acc = 0;
            for (j = 1; j * k <= m; j++) acc += g[j] * b[m - j * k];
            b[m] += acc;
        }
    }
    mem_free(g);
}

static int a1384_is_fixed(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x = x * 2;
    return x == 0;
}

static seq_t *a1384_cache = 0;
static idx_t a1384_cache_n = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (a1384_cache != 0 && n <= a1384_cache_n) return a1384_cache[n];

    int fixed = a1384_is_fixed();
    if (fixed && n > 1000) return 0; /* value cannot fit in any fixed width */

    idx_t N = n;
    seq_t *r = a1384_zeros(N + 1);
    seq_t *e = a1384_zeros(N + 1);
    idx_t m;
    int lev;
    r[1] = 1; /* R_0 = x */
    for (lev = 1; lev <= 4; lev++) {
        if (fixed) a1384_euler_mc(r, e, N - 1);
        else a1384_euler_div(r, e, N - 1);
        r[0] = 0;
        for (m = 1; m <= N; m++) r[m] = e[m - 1];
    }
    mem_free(e);
    r[0] = 1;
    if (a1384_cache != 0) mem_free(a1384_cache);
    a1384_cache = r;
    a1384_cache_n = N;
    return r[n];
}
