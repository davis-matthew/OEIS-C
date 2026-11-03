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
 * Validated: standard, pass, 43 terms through n = 958 (2026-11-03)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

/* returns e*num/j, assuming the result is an integer, without overflowing */
static seq_t step_div(seq_t e, seq_t num, idx_t j) {
    unsigned long long r = (unsigned long long)(e % (seq_t)j);
    unsigned long long g = gcd_u64(r, (unsigned long long)j);
    return (e / (seq_t)g) * (num / (seq_t)(j / g));
}

/* C(k+4,5) */
static seq_t binom5(idx_t k) {
    seq_t r = 1;
    idx_t i;
    for (i = 1; i <= 5; i++) r = step_div(r, (seq_t)(k - 1 + i), i);
    return r;
}

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
    F = mem_new(seq_t, (size_t)(Fcap + 1));
    e = mem_new(seq_t, (size_t)(Fcap + 1));
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
    mem_free(e);
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
    nc = mem_new(seq_t, (size_t)(newcap + 1));
    for (m = 0; m <= newcap; m++) nc[m] = 0;
    for (d = 1; d <= newcap; d++) {
        seq_t v = (seq_t)d * binom5(d);
        for (m = d; m <= newcap; m += d) nc[m] += v;
    }
    if (Cc) mem_free(Cc);
    Cc = nc;
    Ccap = newcap;
}

static void ensure_a(idx_t N) {
    idx_t i, k;
    if (A == NULL) {
        Acap = 16;
        A = mem_new(seq_t, (size_t)(Acap + 1));
        for (i = 0; i <= Acap; i++) A[i] = 0;
        A[0] = 1;
        Acomp = 0;
    }
    if (N > Acap) {
        idx_t newcap = 2 * Acap;
        seq_t *na;
        if (newcap < N) newcap = N;
        na = mem_new(seq_t, (size_t)(newcap + 1));
        for (i = 0; i <= newcap; i++) na[i] = 0;
        for (i = 0; i <= Acomp; i++) na[i] = A[i];
        mem_free(A);
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
