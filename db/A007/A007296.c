/*
 * A007296  Reversion of (1 + g.f. for primes).
 * https://oeis.org/A007296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/n) * [x^(n-1)] (1 + Sum_{k>=1} prime(k)*x^k)^(-n)
 *            (Lagrange inversion), i.e. a(n) = [x^n] of the series reversion
 *            of x*(1 + 2x + 3x^2 + 5x^3 + ...). Small n: direct reversion via
 *            powers of g, a(1) = 1, a(m) = -Sum_{k=1}^{m-1} prime(k) * [x^m]
 *            g^(k+1). Large n: power of the series via th...
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static long long *first_primes(idx_t cnt) {
    long long *pr = (long long *)malloc((size_t)(cnt + 1) * sizeof(long long));
    idx_t c = 0;
    long long v;
    for (v = 2; c < cnt; v++) {
        bool isp = true;
        idx_t i;
        for (i = 0; i < c && pr[i] * pr[i] <= v; i++) {
            if (v % pr[i] == 0) { isp = false; break; }
        }
        if (isp) pr[c++] = v;
    }
    return pr;
}

#define SMALLN 64

static seq_t P[SMALLN + 1][SMALLN + 1];
static bool small_done = false;

static void small_init(void) {
    long long *pr = first_primes(SMALLN);
    int m, j, i, k;
    P[1][1] = 1;
    for (m = 2; m <= SMALLN; m++) {
        for (j = 2; j <= m; j++) {
            seq_t s = 0;
            for (i = 1; i <= m - j + 1; i++)
                s += P[1][i] * P[j - 1][m - i];
            P[j][m] = s;
        }
        {
            seq_t s = 0;
            for (k = 1; k <= m - 1; k++)
                s += P[k + 1][m] * (seq_t)pr[k - 1];
            P[1][m] = -s;
        }
    }
    free(pr);
    small_done = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n <= SMALLN) {
        if (!small_done) small_init();
        return P[1][n];
    }
    {
        long long *pr = first_primes(n);
        seq_t *Q = alloc_zero(n);
        seq_t r;
        idx_t m, k;
        long long nn = (long long)n;
        Q[0] = 1;
        for (m = 1; m < n; m++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++) {
                long long c = (1 - nn) * (long long)k - (long long)m;
                c *= pr[k - 1];
                s += Q[m - k] * (seq_t)c;
            }
            Q[m] = s / (seq_t)(long long)m;
        }
        r = Q[n - 1] / (seq_t)nn;
        release(Q, n);
        free(pr);
        return r;
    }
}
