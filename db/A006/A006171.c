/*
 * A006171  Number of factorization patterns of polynomials of degree n over
 *          integers.
 * https://oeis.org/A006171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   G.f.: Product_{k>=1} (1 - x^k)^(-tau(k)) = Product_{m>=1}
 *            P(x^m), P = partition g.f.; big build: a(n) = (1/n)*Sum_{k=1..n}
 *            b(k)*a(n-k), a(0) = 1, b(k) = Sum_{d|k} d*tau(d); fixed-width
 *            build: multiply by 1/Product_j(1-x^(m*j)) for each m via Euler's
 *            pentagonal theorem
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 63, u64 n <= 180,
 *            u128 n <= 548, big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

/* ---------- fixed-width mode: only ring operations, exact modulo 2^w ---------- */
static seq_t *F = NULL;
static idx_t flen = 0;

static void fixed_build(idx_t size) {
    seq_t *c = alloc_zero(size);
    idx_t N = size - 1;
    c[0] = 1;
    for (idx_t m = 1; m <= N; m++) {
        for (idx_t i = m; i <= N; i++) {
            seq_t s = c[i];
            for (idx_t k = 1;; k++) {
                idx_t g1 = k * (3 * k - 1) / 2;
                if (m * g1 > i) break;
                seq_t t = c[i - m * g1];
                idx_t g2 = k * (3 * k + 1) / 2;
                if (m * g2 <= i) t += c[i - m * g2];
                if (k & 1) s += t; else s -= t;
            }
            c[i] = s;
        }
    }
    if (F) release(F, flen);
    F = c;
    flen = size;
}

/* ---------- arbitrary-precision mode: incremental recurrence ---------- */
static seq_t *A = NULL;
static idx_t acap = 0, alen = 0;
static seq_t *B = NULL;
static idx_t blen = 0;

static void build_b(idx_t size) {
    uint64_t *tau = (uint64_t *)calloc(size, sizeof(uint64_t));
    uint64_t *bn = (uint64_t *)calloc(size, sizeof(uint64_t));
    for (idx_t d = 1; d < size; d++)
        for (idx_t k = d; k < size; k += d) tau[k]++;
    for (idx_t d = 1; d < size; d++) {
        uint64_t w = (uint64_t)d * tau[d];
        for (idx_t k = d; k < size; k += d) bn[k] += w;
    }
    if (B) release(B, blen);
    B = alloc_zero(size);
    blen = size;
    for (idx_t k = 1; k < size; k++) B[k] = (seq_t)bn[k];
    free(tau);
    free(bn);
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        if (n >= flen) {
            idx_t size = flen + flen / 2;
            if (size < n + 1) size = n + 1;
            if (size < 64) size = 64;
            fixed_build(size);
        }
        return F[n];
    }

    if (n >= acap) {
        idx_t size = 2 * acap;
        if (size < n + 1) size = n + 1;
        if (size < 64) size = 64;
        if (A == NULL) A = alloc_zero(size);
        else A = grow_zero(A, acap, size);
        acap = size;
        build_b(size);
    }
    if (alen == 0) {
        A[0] = 1;
        alen = 1;
    }
    for (idx_t i = alen; i <= n; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= i; k++) s += B[k] * A[i - k];
        A[i] = s / (seq_t)i;
        alen = i + 1;
    }
    return A[n];
}
