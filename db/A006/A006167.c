/*
 * A006167  Number of factorization patterns of polynomials of degree n over
 *          F_2.
 * https://oeis.org/A006167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Euler transform of b(n) = Sum_{d|n, A001037(d) >= n/d} 1, i.e.
 *            a(n) = (1/n) * Sum_{k=1..n} c(k)*a(n-k), a(0) = 1, with c(k) =
 *            Sum_{d|k} d*b(d) (fixed-width builds: product of 1/(1-x^d)^b(d)
 *            by saturating repeated prefix sums)
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 92, u64 n <= 278,
 *            u128 n <= 874, big any n
 * Helpers:   alloc_zero, grow_zero, irr_count, mobius_small, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_is_big.h"
#include "../utils/grow_zero.h"
#include "../utils/alloc_zero.h"
#include "../utils/irr_count.h"
#include "../utils/mobius_small.h"

static long long *B = NULL;
static long long *C = NULL;
static idx_t blen = 0;

static void ensure_bc(idx_t N) {
    if (N + 1 <= blen) return;
    free(B); free(C);
    B = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    C = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    for (idx_t d = 1; d <= N; ++d) {
        long long ir = irr_count((long long)d);
        for (idx_t m = 1; d * m <= N && (long long)m <= ir; ++m) B[d * m]++;
    }
    for (idx_t d = 1; d <= N; ++d) {
        long long v = (long long)d * B[d];
        for (idx_t k = d; k <= N; k += d) C[k] += v;
    }
    blen = N + 1;
}

/* big build: exact memoized recurrence */
static seq_t *A = NULL;
static idx_t alen = 0, acap = 0;

/* fixed-width build: saturating table up to FCAP (a(FCAP) far exceeds 2^128) */
#define FCAP 3000
static seq_t *F = NULL;

static void build_fixed(void) {
    seq_t mx = 0;
    mx -= 1;
    ensure_bc(FCAP);
    F = alloc_zero(FCAP + 1);
    F[0] = 1;
    for (idx_t d = 1; d <= FCAP; ++d) {
        for (long long t = 0; t < B[d]; ++t) {
            for (idx_t k = d; k <= FCAP; ++k) {
                seq_t s = F[k] + F[k - d];
                if (s < F[k]) s = mx;
                F[k] = s;
            }
        }
    }
}

seq_t a(idx_t n) {
    if (!seq_is_big()) {
        if (F == NULL) build_fixed();
        if (n > FCAP) n = FCAP;
        return F[n];
    }
    if (n < alen) return A[n];
    if (n + 1 > acap) {
        idx_t nc = 2 * acap;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        A = grow_zero(A, acap, nc);
        acap = nc;
    }
    ensure_bc(acap - 1);
    if (alen == 0) { A[0] = 1; alen = 1; }
    for (idx_t m = alen; m <= n; ++m) {
        seq_t s = 0;
        for (idx_t k = 1; k <= m; ++k)
            s += (seq_t)C[k] * A[m - k];
        A[m] = s / (seq_t)m;
    }
    alen = n + 1;
    return A[n];
}
