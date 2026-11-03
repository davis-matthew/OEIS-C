/*
 * A006708  Expansion of a modular function for gamma_0(6).
 * https://oeis.org/A006708
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  sign, easy
 * Formula:   G.f. q^(-2) * Product_{k>=1} (1-q^k)^e(k) with e(k) = 1 + 7[2|k]
 *            + 5[3|k] - 13[6|k] (i.e. eta(z)*eta(2z)^7*eta(3z)^5/eta(6z)^13);
 *            computed via n*f(n) = -Sum_{m=1..n} s(m)*f(n-m), s(m) =
 *            Sum_{d|m} d*e(d), f(0)=1, a(n) = f(n+2); fixed-width builds
 *            expand the product mod 2^128
 * Range:     i8 n <= 4, i16 n <= 19, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 38 (2026-11-03)
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

static int ex_k(idx_t k) {
    int e = 1;
    if (k % 2 == 0) e += 7;
    if (k % 3 == 0) e += 5;
    if (k % 6 == 0) e -= 13;
    return e;
}

static unsigned __int128 *fw = NULL;
static idx_t fwlen = 0;

static seq_t *bf = NULL;
static idx_t bfcap = 0, bfcnt = 0;
static idx_t *sv = NULL;

seq_t a(idx_t n) {
    if (n < -2) return 0;
    idx_t m = n + 2;

    if (seq_is_fixed_width()) {
        if (m >= fwlen) {
            idx_t len = 2 * m + 1;
            if (len < 64) len = 64;
            unsigned __int128 *c = (unsigned __int128 *)malloc((size_t)len * sizeof(unsigned __int128));
            for (idx_t i = 0; i < len; i++) c[i] = 0;
            c[0] = 1;
            for (idx_t k = 1; k < len; k++) {
                int e = ex_k(k);
                if (e > 0) {
                    for (int p = 0; p < e; p++)
                        for (idx_t i = len - 1; i >= k; i--) c[i] -= c[i - k];
                } else if (e < 0) {
                    for (int p = 0; p < -e; p++)
                        for (idx_t i = k; i < len; i++) c[i] += c[i - k];
                }
            }
            free(fw);
            fw = c;
            fwlen = len;
        }
        return from_i128((__int128)fw[m]);
    }

    if (m >= bfcap) {
        idx_t cap = 2 * m + 1;
        if (cap < 64) cap = 64;
        if (bf == NULL) bf = alloc_zero(cap);
        else bf = grow_zero(bf, bfcap, cap);
        free(sv);
        sv = (idx_t *)malloc((size_t)cap * sizeof(idx_t));
        for (idx_t i = 0; i < cap; i++) sv[i] = 0;
        for (idx_t d = 1; d < cap; d++) {
            idx_t w = d * (idx_t)ex_k(d);
            if (w == 0) continue;
            for (idx_t j = d; j < cap; j += d) sv[j] += w;
        }
        bfcap = cap;
        if (bfcnt == 0) {
            bf[0] = 1;
            bfcnt = 1;
        }
    }
    while (bfcnt <= m) {
        idx_t j = bfcnt;
        seq_t acc = 0;
        for (idx_t k = 1; k <= j; k++) {
            if (sv[k] != 0) acc += bf[j - k] * (seq_t)sv[k];
        }
        bf[j] = -acc / j;
        bfcnt++;
    }
    return bf[m];
}
