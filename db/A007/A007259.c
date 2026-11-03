/*
 * A007259  Expansion of Product_{m>=1} (1 + q^m)^(-8).
 * https://oeis.org/A007259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f.: Product_{k>0} (1 + x^k)^(-8) = Product_{k odd} (1 -
 *            x^k)^8. Fixed-width builds: multiply out Product_{k odd} (1 -
 *            x^k)^8 modulo 2^128. Arbitrary-precision builds: Euler
 *            transform, a(n) = (1/n) * Sum_{k=1..n} -8*A000593(k)*a(n-k),
 *            a(0) = 1, where A000593(k) is the sum of the odd divisors of k
 * Range:     i8 n <= 3, i16 n <= 13, i32 n <= 49, i64 n <= 179, i128 n <= 670,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 9125 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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

static unsigned __int128 *fw_c = NULL;
static idx_t fw_len = 0;

static seq_t a_fixed(idx_t n) {
    if (n >= fw_len) {
        idx_t L = n + 1;
        if (L < 2 * fw_len) L = 2 * fw_len;
        if (L < 64) L = 64;
        unsigned __int128 *c = (unsigned __int128 *)realloc(fw_c, (size_t)L * sizeof(unsigned __int128));
        if (!c) return 0;
        fw_c = c;
        for (idx_t i = 0; i < L; i++) c[i] = 0;
        c[0] = 1;
        for (idx_t k = 1; k < L; k += 2) {
            for (int r = 0; r < 8; r++) {
                for (idx_t i = L - 1; i >= k; i--) {
                    c[i] -= c[i - k];
                }
            }
        }
        fw_len = L;
    }
    return u128_to_seq(fw_c[n]);
}

static seq_t *bg_a = NULL;
static idx_t bg_len = 0, bg_cap = 0;
static long long *bg_so = NULL;

static seq_t a_big(idx_t n) {
    if (n >= bg_cap) {
        idx_t nc = n + 1;
        if (nc < 2 * bg_cap) nc = 2 * bg_cap;
        if (nc < 64) nc = 64;
        bg_a = grow_zero(bg_a, bg_cap, nc);
        long long *so = (long long *)realloc(bg_so, (size_t)nc * sizeof(long long));
        if (!so) return 0;
        bg_so = so;
        for (idx_t i = 0; i < nc; i++) so[i] = 0;
        for (idx_t d = 1; d < nc; d += 2)
            for (idx_t m = d; m < nc; m += d)
                so[m] += (long long)d;
        bg_cap = nc;
    }
    if (bg_len == 0) {
        bg_a[0] = 1;
        bg_len = 1;
    }
    for (idx_t m = bg_len; m <= n; m++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++) {
            s += bg_a[m - k] * (seq_t)(-8LL * bg_so[k]);
        }
        bg_a[m] = s / (seq_t)m;
        bg_len = m + 1;
    }
    return bg_a[n];
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed(n);
    return a_big(n);
}
