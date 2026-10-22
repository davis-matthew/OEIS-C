/*
 * A003705  E.g.f. tan(sin(x)) (odd powers only).
 * https://oeis.org/A003705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2n+1)! [x^(2n+1)] tan(sin(x)). With y = tan(sin x): y' =
 *            cos(x)*(1 + y^2), y(0) = 0. In e.g.f. coefficients y_N: q_N =
 *            Sum_{i=0..N} binomial(N,i)*y_i*y_{N-i}, y_{N+1} = Sum_{i even}
 *            binomial(N,i)*(-1)^(i/2)*([N-i==0] + q_{N-i}), y_0 = 0, y_1 = 1;
 *            a(n) = y_{2n+1}
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 19,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *Y = NULL;   /* e.g.f. coefficients of tan(sin x) */
static seq_t *Q = NULL;   /* e.g.f. coefficients of tan(sin x)^2 */
static seq_t *P = NULL;   /* current Pascal row */
static idx_t cap = 0;     /* allocated length of the arrays */
static idx_t len = 0;     /* Y[0..len-1] known */
static idx_t prow = 0;    /* P holds row prow */

seq_t a(idx_t n) {
    idx_t T = 2 * n + 1;
    if (T + 1 > cap) {
        idx_t nc = T + 1;
        if (nc < 2 * cap) nc = 2 * cap;
        Y = grow_zero(Y, cap, nc);
        Q = grow_zero(Q, cap, nc);
        P = grow_zero(P, cap, nc);
        cap = nc;
    }
    if (len == 0) {
        Y[0] = 0;
        Y[1] = 1;
        Q[0] = 0;
        P[0] = 1;
        prow = 0;
        len = 2;
    }
    while (len <= T) {
        idx_t N = len - 1;
        idx_t i, j;
        seq_t s, t;
        while (prow < N) {
            prow++;
            P[prow] = 1;
            for (j = prow - 1; j >= 1; j--) P[j] += P[j - 1];
        }
        /* q_N: y is odd, so only odd i with even N contribute */
        s = 0;
        if (N % 2 == 0) {
            for (i = 1; i < N; i += 2) s += P[i] * Y[i] * Y[N - i];
        }
        Q[N] = s;
        /* y_{N+1} */
        t = 0;
        if (N % 2 == 0) {
            for (i = 0; i <= N; i += 2) {
                seq_t v = P[i] * Q[N - i];
                if (i == N) v += 1;
                if ((i / 2) % 2) t -= v;
                else t += v;
            }
        }
        Y[len] = t;
        len++;
    }
    return Y[T];
}
