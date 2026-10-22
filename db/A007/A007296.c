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
 * Helpers:   alloc_zero, first_primes, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/first_primes.h"

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
