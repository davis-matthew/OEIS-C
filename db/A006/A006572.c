/*
 * A006572  Numerators of an asymptotic expansion for the number of forests on
 *          n nodes (A001858).
 * https://oeis.org/A006572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator((-1)^n * Sum_{q=n..2n-2} d(q, q-n+1) * (-1/2)^p
 *            / p!), p = floor((q-1)/2), where d(m,k) = A008306(m,k) are the
 *            associated Stirling numbers of the first kind, d(m,k) =
 *            (m-1)*(d(m-1,k) + d(m-2,k-1)), d(0,0) = 1. This is the
 *            coefficient of n^-(n-2) in F_n ~ sqrt(e) n^(n-2) (1 + 5/(2...
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 9, i64 n <= 17, i128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
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

static __int128 g128(__int128 x, __int128 y) {
    if (x < 0) x = -x;
    if (y < 0) y = -y;
    while (y != 0) { __int128 t = x % y; x = y; y = t; }
    return x;
}

static __int128 a_small(idx_t n) {
    idx_t L = n - 1, len = 2 * n + 2, l, q;
    __int128 *prev = (__int128 *)calloc(len, sizeof(__int128));
    __int128 *cur = (__int128 *)calloc(len, sizeof(__int128));
    __int128 *tmp, num = 0, den = 1, w = 1, g;
    idx_t p;
    prev[0] = 1;
    for (l = 1; l <= L; l++) {
        cur[l] = 0;
        cur[2 * l + 1] = 0;
        for (q = l + 1; q <= 2 * l; q++)
            cur[q] = (__int128)(q - 1) * (prev[q - 1] + prev[q - 2]);
        tmp = prev; prev = cur; cur = tmp;
    }
    for (p = 1; p <= n - 2; p++) den *= (__int128)(2 * p);
    p = n - 2;
    for (;;) {
        __int128 s = 0;
        if (2 * p + 1 >= n) s += prev[2 * p + 1];
        if (2 * p + 2 >= n) s += prev[2 * p + 2];
        if (p % 2 == 1) num -= w * s; else num += w * s;
        if (p == 0 || 2 * p + 2 < n) break;
        w *= (__int128)(2 * p);
        p--;
    }
    free(prev);
    free(cur);
    g = g128(num, den);
    if (g != 0) num /= g;
    if (n % 2 == 1) num = -num;
    return num;
}

static seq_t a_big(idx_t n) {
    idx_t L = n - 1, len = 2 * n + 2, l, q;
    seq_t *prev = alloc_zero(len);
    seq_t *cur = alloc_zero(len);
    seq_t *tmp;
    seq_t num = 0, den = 1, w = 1, g;
    idx_t p;
    prev[0] = 1;
    for (l = 1; l <= L; l++) {
        cur[l] = 0;
        cur[2 * l + 1] = 0;
        for (q = l + 1; q <= 2 * l; q++)
            cur[q] = (seq_t)(q - 1) * (prev[q - 1] + prev[q - 2]);
        tmp = prev; prev = cur; cur = tmp;
    }
    for (p = 1; p <= n - 2; p++) den *= (seq_t)(2 * p);
    p = n - 2;
    for (;;) {
        seq_t s = 0;
        if (2 * p + 1 >= n) s += prev[2 * p + 1];
        if (2 * p + 2 >= n) s += prev[2 * p + 2];
        if (p % 2 == 1) num -= w * s; else num += w * s;
        if (p == 0 || 2 * p + 2 < n) break;
        w *= (seq_t)(2 * p);
        p--;
    }
    release(prev, len);
    release(cur, len);
    g = gcd(num < 0 ? -num : num, den);
    if (g != 0) num /= g;
    if (n % 2 == 1) num = -num;
    return num;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    if (seq_is_fixed_width()) return from_i128(a_small(n));
    return a_big(n);
}
