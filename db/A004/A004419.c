/*
 * A004419  Expansion of (Sum_{n=-inf..inf} x^(n^2))^(-18).
 * https://oeis.org/A004419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   with theta(x) = 1 + 2*Sum_{j>=1} x^(j^2) and g = theta^(-18),
 *            J.C.P. Miller power recurrence: a(0) = 1, a(n) = -(2/n) *
 *            Sum_{j>=1, j^2<=n} (17*j^2 + n) * a(n - j^2). (For fixed-width
 *            seq_t, the equivalent product theta(-x)^(-18) = Prod_{m>=1}
 *            ((1+x^m)/(1-x^m))^18 is used instead, since it has onl...
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 25, i128 n <= 77,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 9595 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

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

#define FIXED_LIMIT 1000

static seq_t *fcache = NULL;   /* fixed width: |a(n)| for n < FIXED_LIMIT */

static seq_t *bcache = NULL;   /* big: a(n) for n < bcache_len */
static idx_t bcache_len = 0;
static idx_t bcache_cap = 0;

static void build_fixed(void) {
    idx_t limit = FIXED_LIMIT;
    seq_t *g = alloc_zero(limit);
    g[0] = 1;
    for (idx_t m = 1; m < limit; m++) {
        int t;
        /* multiply by (1 + x^m)^18 */
        for (t = 0; t < 18; t++) {
            for (idx_t i = limit - 1; i >= m; i--) {
                g[i] += g[i - m];
            }
        }
        /* multiply by (1 - x^m)^(-18) */
        for (t = 0; t < 18; t++) {
            for (idx_t i = m; i < limit; i++) {
                g[i] += g[i - m];
            }
        }
    }
    fcache = g;
}

static void ensure_big(idx_t n) {
    if (bcache_len > n) return;
    if (bcache_cap <= n) {
        idx_t newcap = n + 1;
        if (newcap < 2 * bcache_cap) newcap = 2 * bcache_cap;
        if (newcap < 64) newcap = 64;
        if (bcache == NULL) {
            bcache = alloc_zero(newcap);
        } else {
            bcache = grow_zero(bcache, bcache_cap, newcap);
        }
        bcache_cap = newcap;
    }
    if (bcache_len == 0) {
        bcache[0] = 1;
        bcache_len = 1;
    }
    for (idx_t k = bcache_len; k <= n; k++) {
        seq_t s = 0;
        for (idx_t j = 1; j * j <= k; j++) {
            idx_t q = j * j;
            seq_t c = (seq_t)17 * (seq_t)q + (seq_t)k;
            s += c * bcache[k - q];
        }
        s *= 2;
        s /= (seq_t)k;
        bcache[k] = -s;
        bcache_len = k + 1;
    }
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        if (fcache == NULL) build_fixed();
        if (n >= FIXED_LIMIT) return 0; /* far beyond any fixed-width range */
        seq_t v = fcache[n];
        if (n & 1) return -v;
        return v;
    }
    ensure_big(n);
    return bcache[n];
}
