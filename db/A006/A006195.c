/*
 * A006195  Reversion of Jacobi theta_3.
 * https://oeis.org/A006195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^(n+1)] of the series reversion of x*theta_3(x), where
 *            theta_3(x) = 1 + 2*Sum_{k>=1} x^(k^2) (A000122). By Lagrange
 *            inversion, a(n) = [y^n] theta_3(y)^(-(n+1)) / (n+1). The
 *            coefficients p_i of P = T^alpha with T(0) = 1, alpha = -(n+1),
 *            come from i*p_i = Sum_{j=1..i} ((alpha+1)*j - i)*t_j...
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   alloc_zero, lag_i128, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/lag_i128.h"

static seq_t lag_seq(idx_t n) {
    seq_t *p = alloc_zero(n + 1);
    seq_t r;
    p[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k * k <= i; k++) {
            idx_t j = k * k;
            seq_t c = (seq_t)n * (seq_t)j + (seq_t)i;
            s -= (seq_t)2 * c * p[i - j];
        }
        p[i] = s / (seq_t)i;
    }
    r = p[n] / ((seq_t)n + 1);
    release(p, n + 1);
    return r;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width() && sizeof(seq_t) < sizeof(__int128))
        return (seq_t)lag_i128(n);
    return lag_seq(n);
}
