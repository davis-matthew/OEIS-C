/*
 * A006177  Witt vector *2!/2!.
 * https://oeis.org/A006177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Inverse Somos transform of A000108: Product_{i>=1}
 *            1/(1-a(i)*x^i) = Sum_{n>=0} A000108(n)*x^n. Equivalently,
 *            Sum_{d|n} d*a(d)^(n/d) = binomial(2n-1,n), so a(n) =
 *            (binomial(2n-1,n) - Sum_{d|n, d<n} d*a(d)^(n/d)) / n, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, ipow_s, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/ipow_s.h"

static seq_t *A = NULL;   /* A[1..have] valid */
static idx_t have = 0;
static idx_t cap = 0;     /* allocated length of A */
static seq_t bcur;        /* binomial(2*have-1, have) for big path */

/* Fixed width: ring-only operations (exact modulo 2^w). */
static void compute_fixed(idx_t N) {
    seq_t *cat = alloc_zero(N + 1);
    seq_t *P = alloc_zero(N + 1);
    seq_t *nA = alloc_zero(N + 1);
    idx_t k, i, m, j;
    cat[0] = 1;
    for (k = 1; k <= N; k++) {
        seq_t s = 0;
        for (i = 0; i < k; i++) s += cat[i] * cat[k - 1 - i];
        cat[k] = s;
    }
    P[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t am = cat[m] - P[m];
        nA[m] = am;
        for (j = m; j <= N; j++) P[j] += am * P[j - m];
    }
    release(cat, N + 1);
    release(P, N + 1);
    if (A) release(A, cap);
    A = nA;
    cap = N + 1;
    have = N;
}

static void add_term(seq_t *s, idx_t d, idx_t m) {
    *s -= (seq_t)d * ipow_s(A[d], m / d);
}

static void compute_big(idx_t n) {
    idx_t m, d;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 64;
        while (nc < n + 1) nc *= 2;
        A = A ? grow_zero(A, cap, nc) : alloc_zero(nc);
        cap = nc;
    }
    for (m = have + 1; m <= n; m++) {
        seq_t s;
        if (m == 1) bcur = 1;
        else bcur = bcur * (seq_t)(2 * (2 * m - 1)) / (seq_t)m;
        s = bcur;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t f = m / d;
                if (d < m) add_term(&s, d, m);
                if (f != d && f < m) add_term(&s, f, m);
            }
        }
        A[m] = s / (seq_t)m;
    }
    have = n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > have) {
        if (seq_is_fixed_width()) {
            idx_t N = have * 2;
            if (N < 64) N = 64;
            if (N < n) N = n;
            compute_fixed(N);
        } else {
            compute_big(n);
        }
    }
    return A[n];
}
