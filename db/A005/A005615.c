/*
 * A005615  Number of non-degenerate fanout-free Boolean functions of n
 *          variables using And, Or, Not and Majority gates.
 * https://oeis.org/A005615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. A(x) = 2 + B(x), B = Series_Reversion(log(1+x) - x/2 -
 *            x^3/12). Differentiating F(B(x)) = x gives B'*(2 - 2B - B^2 -
 *            B^3) = 4*(1+B). With B = 2G (G has integer e.g.f. coefficients
 *            g(n)): G' = 1 + 2G + 2*G*G' + 2*G^2*G' + 4*G^3*G', so with
 *            binomial convolutions g(n+1) = [n=0] + 2g(n) + 2(G*...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *g_ = 0;   /* g(k) = e.g.f. coefficients of G */
static seq_t *s_ = 0;   /* coefficients of G^2 */
static seq_t *t_ = 0;   /* coefficients of G^3 */
static seq_t *row_ = 0; /* current Pascal row */
static idx_t cap_ = 0;
static idx_t steps_ = 0; /* g known for indices 0..steps_ */

static void ensure(idx_t n) {
    if (n + 2 > cap_) {
        idx_t nc = cap_ * 2;
        if (nc < n + 2) nc = n + 2;
        g_ = grow_zero(g_, cap_, nc);
        s_ = grow_zero(s_, cap_, nc);
        t_ = grow_zero(t_, cap_, nc);
        row_ = grow_zero(row_, cap_, nc);
        cap_ = nc;
    }
    for (idx_t m = steps_; m < n; m++) {
        /* row_ := binomial row m */
        if (m == 0) {
            row_[0] = 1;
        } else {
            for (idx_t k = m; k >= 1; k--) row_[k] += row_[k - 1];
        }
        /* s(m), t(m) */
        seq_t sv = 0, tv = 0;
        for (idx_t k = 1; k < m; k++) {
            sv += row_[k] * g_[k] * g_[m - k];
            tv += row_[k] * g_[k] * s_[m - k];
        }
        s_[m] = sv;
        t_[m] = tv;
        /* g(m+1) */
        seq_t c1 = 0, c2 = 0, c3 = 0;
        for (idx_t k = 1; k <= m; k++) {
            seq_t p = row_[k] * g_[m - k + 1];
            c1 += p * g_[k];
            c2 += p * s_[k];
            c3 += p * t_[k];
        }
        seq_t v = 2 * g_[m] + 2 * c1 + 2 * c2 + 4 * c3;
        if (m == 0) v += 1;
        g_[m + 1] = v;
    }
    if (n > steps_) steps_ = n;
}

seq_t a(idx_t n) {
    if (n == 0) return 2;
    ensure(n);
    return 2 * g_[n];
}
