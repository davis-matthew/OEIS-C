/*
 * A007249  McKay-Thompson series of class 4D for the Monster group.
 * https://oeis.org/A007249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f.: Product_{m>=1} (1 + x^m)^(-12) = Product_{m odd} (1 -
 *            x^m)^12. Fixed-width builds expand the product directly; every
 *            intermediate coefficient is bounded by |a(n)|, so nothing
 *            overflows early. Big-integer builds use a(0) = 1, a(n) =
 *            -(12/n)*Sum_{k=1..n} A000593(k)*a(n-k)
 * Range:     i8 n <= 2, i16 n <= 8, i32 n <= 32, i64 n <= 117, i128 n <= 443,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 994 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
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

static seq_t *a007249_cache = NULL;
static idx_t a007249_len = 0;
static uint64_t *a007249_sig = NULL;

static void a007249_fixed(idx_t need) {
    idx_t N = a007249_len * 2;
    if (N < need) N = need;
    if (a007249_cache != NULL) release(a007249_cache, a007249_len);
    a007249_cache = alloc_zero(N);
    a007249_cache[0] = 1;
    for (idx_t m = 1; m < N; m += 2) {
        for (int r = 0; r < 12; r++) {
            for (idx_t i = N - 1; i >= m; i--) {
                a007249_cache[i] -= a007249_cache[i - m];
            }
        }
    }
    a007249_len = N;
}

static void a007249_big(idx_t need) {
    idx_t N = need;
    if (a007249_cache == NULL) {
        a007249_cache = alloc_zero(N);
        a007249_cache[0] = 1;
        a007249_len = 1;
    } else {
        a007249_cache = grow_zero(a007249_cache, a007249_len, N);
    }
    free(a007249_sig);
    a007249_sig = (uint64_t *)calloc((size_t)N, sizeof(uint64_t));
    for (idx_t d = 1; d < N; d += 2) {
        for (idx_t j = d; j < N; j += d) a007249_sig[j] += d;
    }
    for (idx_t i = a007249_len; i < N; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= i; k++) {
            s += (seq_t)a007249_sig[k] * a007249_cache[i - k];
        }
        a007249_cache[i] = (s * -12) / (seq_t)i;
    }
    a007249_len = N;
}

seq_t a(idx_t n) {
    if (n >= a007249_len) {
        if (seq_is_fixed_width()) a007249_fixed(n + 1);
        else a007249_big(n + 1);
    }
    return a007249_cache[n];
}
