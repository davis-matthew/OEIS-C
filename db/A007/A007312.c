/*
 * A007312  Reversion of g.f. (with constant term omitted) for partition
 *          numbers.
 * https://oeis.org/A007312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/n) * [x^(n-1)] G(x)^(-n), where G(x) = Sum_{j>=0}
 *            p(j+1) x^j and p = A000041 (Lagrange inversion); h = G^(-n) via
 *            h(0) = 1, h(k) = (1/k) * Sum_{j=1..k} ((1-n)*j - k) * p(j+1) *
 *            h(k-j)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 16, i64 n <= 30, i128 n <= 56,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 916 (2026-11-03)
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

static seq_t from_i128(__int128 v)
{
    if (v >= -(__int128)4000000000000000000LL && v <= (__int128)4000000000000000000LL)
        return (seq_t)(long long)v;
    {
        __int128 q = v / ((__int128)1 << 32);
        long long rem = (long long)(v - q * ((__int128)1 << 32));
        return from_i128(q) * (seq_t)4294967296LL + (seq_t)rem;
    }
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

static void parts_i128(__int128 *p, idx_t m) {
    p[0] = 1;
    for (idx_t i = 1; i <= m; i++) {
        __int128 s = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                s += p[i - g1];
                if (g2 <= i) s += p[i - g2];
            } else {
                s -= p[i - g1];
                if (g2 <= i) s -= p[i - g2];
            }
        }
        p[i] = s;
    }
}

static __int128 a_i128(idx_t n) {
    __int128 *p = (__int128 *)malloc((size_t)(n + 1) * sizeof(__int128));
    __int128 *h = (__int128 *)malloc((size_t)n * sizeof(__int128));
    parts_i128(p, n);
    __int128 nn = (__int128)n;
    h[0] = 1;
    for (idx_t k = 1; k < n; k++) {
        __int128 s = 0;
        for (idx_t j = 1; j <= k; j++) {
            __int128 c = (__int128)j * (1 - nn) - (__int128)k;
            s += c * p[j + 1] * h[k - j];
        }
        h[k] = s / (__int128)k;
    }
    __int128 r = h[n - 1] / nn;
    free(p);
    free(h);
    return r;
}

static void parts_seq(seq_t *p, idx_t m) {
    p[0] = 1;
    for (idx_t i = 1; i <= m; i++) {
        seq_t s = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                s += p[i - g1];
                if (g2 <= i) s += p[i - g2];
            } else {
                s -= p[i - g1];
                if (g2 <= i) s -= p[i - g2];
            }
        }
        p[i] = s;
    }
}

static seq_t a_seq(idx_t n) {
    seq_t *p = alloc_zero(n + 1);
    seq_t *h = alloc_zero(n);
    parts_seq(p, n);
    seq_t nn = (seq_t)n;
    h[0] = 1;
    for (idx_t k = 1; k < n; k++) {
        seq_t s = 0;
        seq_t kk = (seq_t)k;
        for (idx_t j = 1; j <= k; j++) {
            seq_t c = (seq_t)j * (1 - nn) - kk;
            s += (c * p[j + 1]) * h[k - j];
        }
        h[k] = s / kk;
    }
    seq_t r = h[n - 1] / nn;
    release(p, n + 1);
    release(h, n);
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (seq_is_fixed_width()) return from_i128(a_i128(n));
    return a_seq(n);
}
