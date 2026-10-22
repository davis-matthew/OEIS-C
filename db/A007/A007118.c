/*
 * A007118  Expansion of e.g.f. (1+x)^sin(x).
 * https://oeis.org/A007118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   E.g.f. f(x) = (1+x)^sin(x) = exp(g(x)), g(x) = sin(x)*log(1+x),
 *            so f' = g'*f and a(n+1) = Sum_{k=0..n}
 *            binomial(n,k)*G(k+1)*a(n-k), a(0) = 1, where G(m) = m![x^m] g(x)
 *            = Sum_{j odd, j<m}
 *            binomial(m,j)*(-1)^((j-1)/2)*(-1)^(m-j-1)*(m-j-1)!
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *A = 0, *Gv = 0, *F = 0, *R0 = 0, *R1 = 0;
static idx_t cnt = 0, cap = 0;

static void ensure(idx_t need) {
    idx_t nc;
    if (need <= cap) return;
    nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    if (cap == 0) {
        A = alloc_zero(nc);
        Gv = alloc_zero(nc);
        F = alloc_zero(nc);
        R0 = alloc_zero(nc);
        R1 = alloc_zero(nc);
    } else {
        A = grow_zero(A, cap, nc);
        Gv = grow_zero(Gv, cap, nc);
        F = grow_zero(F, cap, nc);
        R0 = grow_zero(R0, cap, nc);
        R1 = grow_zero(R1, cap, nc);
    }
    cap = nc;
}

seq_t a(idx_t n) {
    idx_t m, j, k;
    ensure(n + 2);
    if (cnt == 0) {
        A[0] = 1;
        F[0] = 1;
        Gv[0] = 0;
        R0[0] = 1;
        cnt = 1;
    }
    for (m = cnt; m <= n; m++) {
        seq_t g = 0, s = 0;
        seq_t *tmp;
        F[m] = F[m - 1] * (seq_t)m;
        /* R1 = Pascal row m from R0 = row m-1 */
        R1[0] = 1;
        for (j = 1; j < m; j++) R1[j] = R0[j - 1] + R0[j];
        R1[m] = 1;
        for (j = 1; j < m; j += 2) {
            seq_t t = R1[j] * F[m - j - 1];
            if ((((j - 1) / 2) + (m - j - 1)) & 1) g -= t;
            else g += t;
        }
        Gv[m] = g;
        for (k = 0; k < m; k++) {
            if (Gv[k + 1] != 0) s += R0[k] * Gv[k + 1] * A[m - 1 - k];
        }
        A[m] = s;
        tmp = R0; R0 = R1; R1 = tmp;
        cnt = m + 1;
    }
    return A[n];
}
