/*
 * A007245  McKay-Thompson series of class 3C for the Monster group.
 * https://oeis.org/A007245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f. = E_4(x) / f(-x)^8, i.e. A(x)^3 = j, so a(n) = Sum_{k=0..n}
 *            e(k) * p8(n-k), with e(0) = 1, e(k) = 240*sigma_3(k), and p8 =
 *            A023007 (coefficients of Product_{k>=1} 1/(1-x^k)^8), where
 *            n*p8(n) = 8*Sum_{k=1..n} sigma(k)*p8(n-k), p8(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 43, u128 n <= 165,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 9991 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

static seq_t *P = NULL;
static idx_t plen = 0;
static uint64_t *S1 = NULL, *S3 = NULL;
static idx_t slen = 0;

static void ensure_sigma(idx_t len) {
    idx_t d, m;
    if (len <= slen) return;
    free(S1);
    free(S3);
    S1 = (uint64_t *)calloc(len, sizeof(uint64_t));
    S3 = (uint64_t *)calloc(len, sizeof(uint64_t));
    for (d = 1; d < len; d++)
        for (m = d; m < len; m += d) {
            S1[m] += d;
            S3[m] += (uint64_t)d * d * d;
        }
    slen = len;
}

static void ensure_p(idx_t len) {
    if (len <= plen) return;
    if (seq_is_fixed_width()) {
        /* division-free: multiply by 1/(1-x^k) eight times (wraparound-safe) */
        idx_t L = (len < 2 * plen) ? 2 * plen : len;
        idx_t k, i;
        int r;
        if (P) release(P, plen);
        P = alloc_zero(L);
        P[0] = 1;
        for (r = 0; r < 8; r++)
            for (k = 1; k < L; k++)
                for (i = k; i < L; i++)
                    P[i] += P[i - k];
        plen = L;
    } else {
        idx_t m, k;
        ensure_sigma(len);
        if (plen == 0) {
            P = alloc_zero(len);
            P[0] = 1;
            plen = 1;
        } else {
            P = grow_zero(P, plen, len);
        }
        for (m = plen; m < len; m++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++)
                s += P[m - k] * (seq_t)S1[k];
            P[m] = s * 8 / m;
        }
        plen = len;
    }
}

seq_t a(idx_t n) {
    idx_t k;
    seq_t r;
    ensure_sigma(n + 1);
    ensure_p(n + 1);
    r = P[n];
    for (k = 1; k <= n; k++)
        r += P[n - k] * (seq_t)(240 * S3[k]);
    return r;
}
