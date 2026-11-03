/*
 * A005217  Number of unlabeled unit interval graphs with n nodes.
 * https://oeis.org/A005217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   1 + Sum a(n) x^n = exp( Sum_{k>=1} psi(x^k)/k ) (Euler transform
 *            of A007123), where b(k) = A007123(k) = (c(k) + 2*Catalan(k-1))/4
 *            with c(k) = [x^k] (1+2x)/sqrt(1-4x^2) (c(2m) = C(2m,m), c(2m+1)
 *            = 2*C(2m,m)); then with s(k) = Sum_{d|k} d*b(d), a(0) = 1, a(m)
 *            = (1/m) * Sum_{k=1..m} s(k)*a(m-k)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 176 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

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

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
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

static void a5217_fill_big(seq_t *B, seq_t *C, idx_t L)
{
    seq_t cb = 1, cat = 1;
    idx_t k, d, j;
    for (k = 1; k <= L; k++) {
        seq_t c;
        if (k % 2 == 0) {
            idx_t m = k / 2;
            cb = cb * (seq_t)(2 * (2 * m - 1));
            cb /= (seq_t)m;
            c = cb;
        } else {
            c = cb * 2;
        }
        B[k] = (c + cat * 2) / 4;
        cat = cat * (seq_t)(4 * k - 2);
        cat /= (seq_t)(k + 1);
        C[k] = 0;
    }
    for (d = 1; d <= L; d++)
        for (j = d; j <= L; j += d)
            C[j] += B[d] * (seq_t)d;
}

typedef unsigned __int128 a5217_u128;

/* ---------- arbitrary-precision path ---------- */
static seq_t *bA = NULL, *bB = NULL, *bC = NULL;
static idx_t bcap = 0, bhave = 0;

static seq_t *a5217_grow(seq_t *old, idx_t oldlen, idx_t newlen)
{
    if (old == NULL || oldlen == 0) return alloc_zero(newlen);
    return grow_zero(old, oldlen, newlen);
}

static seq_t a5217_big(idx_t n)
{
    if (n + 1 > bcap) {
        idx_t nc = 2 * bcap;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        bA = a5217_grow(bA, bcap, nc);
        bB = a5217_grow(bB, bcap, nc);
        bC = a5217_grow(bC, bcap, nc);
        a5217_fill_big(bB, bC, nc - 1);
        bcap = nc;
    }
    if (bhave == 0) { bA[0] = 1; bhave = 1; }
    while (bhave <= n) {
        idx_t m = bhave, k;
        seq_t s = 0;
        for (k = 1; k <= m; k++) s += bC[k] * bA[m - k];
        bA[m] = s / (seq_t)m;
        bhave++;
    }
    return bA[n];
}

/* ---------- fixed-width path (computed in 128 bits) ---------- */
static a5217_u128 *uA = NULL, *uB = NULL, *uC = NULL;
static idx_t ucap = 0, uhave = 0;

static void a5217_fill_u(a5217_u128 *B, a5217_u128 *C, idx_t L)
{
    a5217_u128 cb = 1, cat = 1;
    idx_t k, d, j;
    for (k = 1; k <= L; k++) {
        a5217_u128 c;
        if (k % 2 == 0) {
            idx_t m = k / 2;
            cb = cb * (a5217_u128)(2 * (2 * m - 1)) / (a5217_u128)m;
            c = cb;
        } else {
            c = cb * 2;
        }
        B[k] = (c + cat * 2) / 4;
        cat = cat * (a5217_u128)(4 * k - 2) / (a5217_u128)(k + 1);
        C[k] = 0;
    }
    for (d = 1; d <= L; d++)
        for (j = d; j <= L; j += d)
            C[j] += B[d] * (a5217_u128)d;
}

static a5217_u128 a5217_u(idx_t n)
{
    if (n + 1 > ucap) {
        idx_t nc = 2 * ucap, i;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        uA = (a5217_u128 *)realloc(uA, nc * sizeof(a5217_u128));
        uB = (a5217_u128 *)realloc(uB, nc * sizeof(a5217_u128));
        uC = (a5217_u128 *)realloc(uC, nc * sizeof(a5217_u128));
        for (i = ucap; i < nc; i++) { uA[i] = 0; uB[i] = 0; uC[i] = 0; }
        a5217_fill_u(uB, uC, nc - 1);
        ucap = nc;
    }
    if (uhave == 0) { uA[0] = 1; uhave = 1; }
    while (uhave <= n) {
        idx_t m = uhave, k;
        a5217_u128 s = 0;
        for (k = 1; k <= m; k++) s += uC[k] * uA[m - k];
        uA[m] = s / (a5217_u128)m;
        uhave++;
    }
    return uA[n];
}

seq_t a(idx_t n)
{
    if (seq_is_big()) return a5217_big(n);
    return u128_to_seq(a5217_u(n));
}
