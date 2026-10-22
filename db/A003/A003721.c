/*
 * A003721  Expansion of e.g.f. tan(tanh(x)) (odd powers only).
 * https://oeis.org/A003721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   f(x) = tan(tanh(x)) = Sum f_m x^m/m!, a(n) = f_{2n+1}, computed
 *            from the ODEs g = tanh(x): g' = 1 - g^2, g(0) = 0; f' = (1 +
 *            f^2) * g', f(0) = 0; i.e. with binomial convolutions: g_{m+1} =
 *            [m=0] - Sum_{j=0..m} binomial(m,j)*g_j*g_{m-j}, S_m = [m=0] +
 *            Sum_{i=0..m} binomial(m,i)*f_i*f_{m-i}, f_{m+1...
 * Range:     i8 n <= 4, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 19,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *G = NULL, *F = NULL, *S = NULL, *R = NULL;
static idx_t cap = 0;
static idx_t have = 0; /* F[0..have] known; R holds Pascal row (have - 1) */

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap * 2;
    if (nc < need) nc = need;
    if (nc < 16) nc = 16;
    G = grow_zero(G, cap, nc);
    F = grow_zero(F, cap, nc);
    S = grow_zero(S, cap, nc);
    R = grow_zero(R, cap, nc);
    if (cap == 0) R[0] = 1;
    cap = nc;
}

static void step(idx_t m) {
    idx_t j;
    if (m > 0) {
        for (j = m; j >= 1; j--) R[j] += R[j - 1];
    }
    S[m] = (m == 0) ? 1 : 0;
    G[m + 1] = (m == 0) ? 1 : 0;
    F[m + 1] = 0;
    if (m % 2 != 0) return;
    for (j = 1; j < m; j += 2) {
        S[m] += R[j] * F[j] * F[m - j];
        G[m + 1] -= R[j] * G[j] * G[m - j];
    }
    for (j = 0; j <= m; j += 2) {
        F[m + 1] += R[j] * S[j] * G[m + 1 - j];
    }
}

seq_t a(idx_t n) {
    idx_t N = 2 * n + 1;
    ensure_cap(N + 2);
    while (have < N) {
        step(have);
        have++;
    }
    return F[N];
}
