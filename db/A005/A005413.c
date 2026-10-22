/*
 * A005413  Number of non-vanishing Feynman diagrams of order 2n+1 for the
 *          electron-electron-photon proper vertex function in quantum
 *          electrodynamics (QED).
 * https://oeis.org/A005413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. A(x) = (U(x) - 1)/(x*U(x)^3), where U(x) = Sum u(n) x^n is
 *            the g.f. of A005411, which satisfies U = 1 + x*U^2 + 2*x^2*U'
 *            (from Coquereaux's Bessel-function form), i.e. u(0) = 1, u(n) =
 *            [x^(n-1)] U^2 + 2*(n-1)*u(n-1). With P = U^2, W = U^3: a(n) =
 *            u(n+1) - Sum_{k=1..n} W(k)*a(n-k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *su = 0, *sP = 0, *sW = 0, *sA = 0;
static idx_t s_cnt = 0, s_cap = 0;

seq_t a(idx_t n)
{
    idx_t m, k;
    if (n < s_cnt)
        return sA[n];
    if (n + 2 > s_cap) {
        idx_t newcap = n + 2;
        if (newcap < 2 * s_cap)
            newcap = 2 * s_cap;
        su = grow_zero(su, s_cap, newcap);
        sP = grow_zero(sP, s_cap, newcap);
        sW = grow_zero(sW, s_cap, newcap);
        sA = grow_zero(sA, s_cap, newcap);
        s_cap = newcap;
    }
    for (m = s_cnt; m <= n; m++) {
        seq_t p = 0, w = 0, s = 0;
        if (m == 0)
            su[0] = 1;
        for (k = 0; k <= m; k++)
            p += su[k] * su[m - k];
        sP[m] = p;
        for (k = 0; k <= m; k++)
            w += sP[k] * su[m - k];
        sW[m] = w;
        su[m + 1] = p + (seq_t)2 * (seq_t)m * su[m];
        for (k = 1; k <= m; k++)
            s += sW[k] * sA[m - k];
        sA[m] = su[m + 1] - s;
    }
    s_cnt = n + 1;
    return sA[n];
}
