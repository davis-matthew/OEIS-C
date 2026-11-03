/*
 * A003169  Number of 2-line arrays; or number of P-graphs with 2n edges.
 * https://oeis.org/A003169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for exact (big-integer) values, a(n) =
 *            ((324*n^2-708*n+360)*a(n-1) - (371*n^2-1831*n+2250)*a(n-2) +
 *            (20*n^2-130*n+210)*a(n-3)) / (16*n*(2*n-1)) for n > 2, a(0) = 0,
 *            a(1) = 1, a(2) = 3. For fixed-width types: from A =
 *            x*(1+A)/(1-A)^2, i.e. A - 2*A^2 + A^3 = x + x*A, a(n) = [n=1] +
 *            a(n-1) + 2*[x^n]...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 188 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *A_grow(seq_t *old, idx_t oldlen, idx_t newlen)
{
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    idx_t i;
    for (i = 0; i < newlen; i++) p[i] = 0;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static int a003169_fixed(void) {
    seq_t t = 1;
    int k;
    for (k = 0; k < 130; k++) t <<= 1;
    return t == 0;
}

static seq_t a003169_from128(unsigned __int128 v) {
    if (sizeof(seq_t) <= 8) return (seq_t)(uint64_t)v;
    {
        seq_t r = (seq_t)(uint64_t)(v >> 64);
        int k;
        for (k = 0; k < 4; k++) r <<= 16;
        r += (seq_t)(uint64_t)v;
        return r;
    }
}

static seq_t a003169_fixed_calc(idx_t n) {
    static unsigned __int128 *A = NULL, *S2 = NULL;
    static idx_t done = 0, cap = 0;
    idx_t m, i;
    if (n >= cap) {
        idx_t nc = n + 64;
        A = (unsigned __int128 *)realloc(A, (size_t)nc * sizeof(unsigned __int128));
        S2 = (unsigned __int128 *)realloc(S2, (size_t)nc * sizeof(unsigned __int128));
        if (cap == 0) { A[0] = 0; S2[0] = 0; }
        cap = nc;
    }
    for (m = done + 1; m <= n; m++) {
        unsigned __int128 s2 = 0, s3 = 0;
        for (i = 1; i < m; i++) s2 += A[i] * A[m - i];
        S2[m] = s2;
        for (i = 1; i + 2 <= m; i++) s3 += A[i] * S2[m - i];
        A[m] = (unsigned __int128)(m == 1 ? 1 : 0) + A[m - 1] + 2 * s2 - s3;
    }
    if (n > done) done = n;
    return a003169_from128(A[n]);
}

static seq_t a003169_big_calc(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cap = 0, done = 2;
    idx_t i;
    if (cache == NULL) {
        cap = (n < 100) ? 100 : n + 10;
        cache = alloc_zero(cap);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
    }
    if (n >= cap) {
        cache = A_grow(cache, cap, n + 10);
        cap = n + 10;
    }
    for (i = done + 1; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t n2 = ni * ni;
        seq_t c1 = (seq_t)324 * n2 + (seq_t)360 - (seq_t)708 * ni;
        seq_t c2 = (seq_t)371 * n2 + (seq_t)2250 - (seq_t)1831 * ni;
        seq_t c3 = (seq_t)20 * n2 + (seq_t)210 - (seq_t)130 * ni;
        seq_t den = (seq_t)16 * ni * ((seq_t)2 * ni - 1);
        seq_t num = c1 * cache[i - 1] + c3 * cache[i - 3];
        num -= c2 * cache[i - 2];
        cache[i] = num / den;
    }
    if (n > done) done = n;
    return cache[n];
}

seq_t a(idx_t n) {
    if (a003169_fixed()) return a003169_fixed_calc(n);
    return a003169_big_calc(n);
}
