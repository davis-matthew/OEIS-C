/*
 * A003700  Expansion of tan(x /cosh(x)).
 * https://oeis.org/A003700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   E.g.f. y(x) = tan(x/cosh(x)) = Sum_n a(n) x^(2n+1)/(2n+1)!. With
 *            u = x*sech(x), y' = (1 + y^2) * u', so with E.g.f. coefficients
 *            y_{m+1} = Sum_{k=0..m} binomial(m,k) * w_k * u'_{m-k}, where w =
 *            1 + y^2, w_m = [m=0] + Sum_k binomial(m,k) y_k y_{m-k}, u'_m =
 *            (m+1)*E(m) (E = Euler numbers, E(m) = -S...
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *Y = NULL;
static idx_t L = 0;

static void build(idx_t M) {
    idx_t len = M + 1, n, k;
    seq_t *row = alloc_zero(len);
    seq_t *e = alloc_zero(len);
    seq_t *up = alloc_zero(len);
    seq_t *w = alloc_zero(len);
    seq_t *y = alloc_zero(len);
    for (n = 0; n < M; n++) {
        if (n == 0) {
            row[0] = 1;
        } else {
            for (k = n; k >= 1; k--) row[k] += row[k - 1];
        }
        if (n & 1) continue; /* odd n: e, up, w are 0 and y[n+1] = 0 */
        if (n == 0) {
            e[0] = 1;
        } else {
            seq_t t = 0;
            for (k = 0; k < n; k += 2) t += row[k] * e[k];
            e[n] = -t;
        }
        up[n] = e[n] * ((seq_t)n + 1);
        {
            seq_t s = 0;
            for (k = 1; k < n; k += 2) s += row[k] * y[k] * y[n - k];
            if (n == 0) s += 1;
            w[n] = s;
        }
        {
            seq_t t = 0;
            for (k = 0; k <= n; k += 2) t += row[k] * w[k] * up[n - k];
            y[n + 1] = t;
        }
    }
    release(row, len);
    release(e, len);
    release(up, len);
    release(w, len);
    if (Y != NULL) release(Y, L);
    Y = y;
    L = len;
}

seq_t a(idx_t n) {
    idx_t need = 2 * n + 1;
    if (need >= L) {
        idx_t M = need;
        if (L > 0 && L + L / 2 > M) M = L + L / 2;
        build(M);
    }
    return Y[need];
}
