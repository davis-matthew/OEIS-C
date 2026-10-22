/*
 * A001942  Expansion of reciprocal of theta series of Leech lattice.
 * https://oeis.org/A001942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f.: 1/Theta_Leech(q), where Theta_Leech = E4^3 - 720*Delta, E4
 *            = 1 + 240*Sum_{k>=1} sigma_3(k) q^k, Delta = q*Product_{k>=1}
 *            (1-q^k)^24 (Product (1-q^k)^3 = Sum_{m>=0} (-1)^m (2m+1)
 *            q^(m(m+1)/2), squared three times). Recurrence: a(0) = 1, a(n) =
 *            -Sum_{k=1..n} theta(k)*a(n-k)
 * Range:     i8 n <= 1, i16 n <= 1, i32 n <= 3, i64 n <= 7, i128 n <= 14,
 *            big any n
 * Helpers:   a1942_sq, alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/a1942_sq.h"

static seq_t *a1942_A = NULL;
static idx_t a1942_cap = 0;

static void a1942_build(idx_t N) {
    seq_t *e4 = alloc_zero(N);
    seq_t *e2 = alloc_zero(N);
    seq_t *e3 = alloc_zero(N);
    seq_t *p3 = alloc_zero(N);
    seq_t *p6 = alloc_zero(N);
    seq_t *p12 = alloc_zero(N);
    seq_t *p24 = alloc_zero(N);
    seq_t *th = alloc_zero(N);
    seq_t *A = alloc_zero(N);

    for (idx_t d = 1; d < N; d++) {
        seq_t c = (seq_t)d * (seq_t)d * (seq_t)d;
        for (idx_t m = d; m < N; m += d) e4[m] += c;
    }
    for (idx_t m = 1; m < N; m++) e4[m] = e4[m] * 240;
    e4[0] = 1;

    a1942_sq(e4, e2, N);
    for (idx_t i = 0; i < N; i++) {
        seq_t s = 0;
        for (idx_t j = 0; j <= i; j++) {
            if (e2[j] == 0) continue;
            s += e2[j] * e4[i - j];
        }
        e3[i] = s;
    }

    for (idx_t m = 0; m * (m + 1) / 2 < N; m++) {
        seq_t v = (seq_t)(2 * m + 1);
        p3[m * (m + 1) / 2] = (m % 2 == 1) ? (0 - v) : v;
    }
    a1942_sq(p3, p6, N);
    a1942_sq(p6, p12, N);
    a1942_sq(p12, p24, N);

    th[0] = 1;
    for (idx_t i = 1; i < N; i++) th[i] = e3[i] - p24[i - 1] * 720;

    A[0] = 1;
    for (idx_t i = 1; i < N; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= i; k++) {
            if (th[k] == 0) continue;
            s += th[k] * A[i - k];
        }
        A[i] = 0 - s;
    }

    a1942_A = A;
    a1942_cap = N;
}

seq_t a(idx_t n) {
    if (n >= a1942_cap) {
        idx_t N = a1942_cap * 2;
        if (N < n + 1) N = n + 1;
        if (N < 64) N = 64;
        a1942_build(N);
    }
    return a1942_A[n];
}
