/*
 * A005616  Number of non-degenerate disjunctively-realizable functions of n
 *          variables.
 * https://oeis.org/A005616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f.: 2*(p + q + 1) where q = exp(p) - p - 1, p = exp(2*q + p
 *            + x) - (2*q + p + 1). With S = 2q + p + x, E = exp(S), F =
 *            exp(p) (e.g.f. coefficients s_n, e_n, f_n): p(n) = [n=1] +
 *            Sum_{k=1..n-1} binomial(n-1,k-1)*s(k)*e(n-k), q(n) =
 *            Sum_{k=1..n-1} binomial(n-1,k-1)*p(k)*f(n-k), s(n) = 2q(n) +
 *            p...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *P = 0, *Q = 0, *S = 0, *E = 0, *F = 0, *row = 0;
static idx_t cnt = 0, cap = 0;

static void ensure_cap(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    P = grow_zero(P, cap, nc);
    Q = grow_zero(Q, cap, nc);
    S = grow_zero(S, cap, nc);
    E = grow_zero(E, cap, nc);
    F = grow_zero(F, cap, nc);
    row = grow_zero(row, cap, nc);
    cap = nc;
}

seq_t a(idx_t n) {
    idx_t m, j, k;
    ensure_cap(n + 2);
    if (cnt == 0) {
        P[0] = 0; Q[0] = 0; S[0] = 0; E[0] = 1; F[0] = 1;
        row[0] = 1; /* row holds binomial(m-1, .) for the last computed m (start: C(0,.)) */
        cnt = 1;
    }
    for (m = cnt; m <= n; m++) {
        seq_t pn, qn;
        /* update row to binomial(m-1, .) */
        if (m >= 2) {
            for (j = m - 1; j >= 1; j--) row[j] += row[j - 1];
        }
        pn = (m == 1) ? 1 : 0;
        qn = 0;
        for (k = 1; k + 1 <= m; k++) {
            pn += row[k - 1] * S[k] * E[m - k];
            qn += row[k - 1] * P[k] * F[m - k];
        }
        P[m] = pn;
        Q[m] = qn;
        S[m] = 2 * qn + pn;
        if (m == 1) S[m] += 1;
        E[m] = pn + S[m];
        if (m == 1) E[m] -= 1;
        F[m] = qn + pn;
        cnt = m + 1;
    }
    if (n == 0) return 2;
    return 2 * (P[n] + Q[n]);
}
