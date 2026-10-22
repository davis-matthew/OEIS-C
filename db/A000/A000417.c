/*
 * A000417  Euler transform of A000389.
 * https://oeis.org/A000417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Euler transform of b(k) = A000389(k+4) = C(k+4,5): a(n) = [x^n]
 *            Product_{k>=1} 1/(1-x^k)^C(k+4,5), a(0) = 1; equivalently n*a(n)
 *            = Sum_{k=1..n} c(k)*a(n-k), c(k) = Sum_{d|k} d*C(d+4,5)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 42, u128 n <= 95,
 *            big any n
 * Helpers:   binom5, gcd_ul, step_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom5.h"
#include "../utils/gcd_ul.h"
#include "../utils/step_div.h"

static int g_width = 0;

static int seq_width(void) {
    if (g_width == 0) {
        seq_t x = 1;
        int w = 0;
        while (x != 0 && w < 1024) { x = x * 2; w++; }
        g_width = w;
    }
    return g_width;
}

/* fixed-width mode: division-free product expansion */
static seq_t *F = NULL;
static idx_t Fcap = 0;

static void build_fixed(void) {
    idx_t i, j, k;
    seq_t *e;
    Fcap = (idx_t)(8 * seq_width());
    F = (seq_t *)malloc((size_t)(Fcap + 1) * sizeof(seq_t));
    e = (seq_t *)malloc((size_t)(Fcap + 1) * sizeof(seq_t));
    for (i = 0; i <= Fcap; i++) F[i] = 0;
    F[0] = 1;
    for (k = 1; k <= Fcap; k++) {
        seq_t b = binom5(k);
        idx_t M = Fcap / k;
        e[0] = 1;
        for (j = 1; j <= M; j++) e[j] = step_div(e[j - 1], b + (seq_t)(j - 1), j);
        for (i = Fcap; i >= k; i--) {
            seq_t s = 0;
            for (j = 1; j * k <= i; j++) s += e[j] * F[i - j * k];
            F[i] += s;
        }
    }
    free(e);
}

/* big mode: logarithmic-derivative recurrence */
static seq_t *A = NULL;
static idx_t Acomp = 0, Acap = 0;
static seq_t *Cc = NULL;
static idx_t Ccap = 0;

static void ensure_c(idx_t N) {
    idx_t newcap, d, m;
    seq_t *nc;
    if (N <= Ccap) return;
    newcap = 2 * Ccap;
    if (newcap < N) newcap = N;
    if (newcap < 16) newcap = 16;
    nc = (seq_t *)malloc((size_t)(newcap + 1) * sizeof(seq_t));
    for (m = 0; m <= newcap; m++) nc[m] = 0;
    for (d = 1; d <= newcap; d++) {
        seq_t v = (seq_t)d * binom5(d);
        for (m = d; m <= newcap; m += d) nc[m] += v;
    }
    if (Cc) free(Cc);
    Cc = nc;
    Ccap = newcap;
}

static void ensure_a(idx_t N) {
    idx_t i, k;
    if (A == NULL) {
        Acap = 16;
        A = (seq_t *)malloc((size_t)(Acap + 1) * sizeof(seq_t));
        for (i = 0; i <= Acap; i++) A[i] = 0;
        A[0] = 1;
        Acomp = 0;
    }
    if (N > Acap) {
        idx_t newcap = 2 * Acap;
        seq_t *na;
        if (newcap < N) newcap = N;
        na = (seq_t *)malloc((size_t)(newcap + 1) * sizeof(seq_t));
        for (i = 0; i <= newcap; i++) na[i] = 0;
        for (i = 0; i <= Acomp; i++) na[i] = A[i];
        free(A);
        A = na;
        Acap = newcap;
    }
    ensure_c(N);
    for (i = Acomp + 1; i <= N; i++) {
        seq_t s = 0;
        for (k = 1; k <= i; k++) s += Cc[k] * A[i - k];
        A[i] = s / (seq_t)i;
    }
    if (N > Acomp) Acomp = N;
}

seq_t a(idx_t n) {
    if (seq_width() < 1024) {
        if (F == NULL) build_fixed();
        if (n > Fcap) return 0;
        return F[n];
    }
    if (n > Acomp) ensure_a(n);
    return A[n];
}
