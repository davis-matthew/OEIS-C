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
 * Helpers:   a5217_fill_big, alloc_zero, grow_zero, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_zero.h"
#include "../utils/alloc_zero.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a5217_fill_big.h"

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
