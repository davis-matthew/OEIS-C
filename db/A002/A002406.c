/*
 * A002406  Coefficients for step-by-step integration.
 * https://oeis.org/A002406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (n+1) * lcm(1..n+2) * Integral_{u=0..1}
 *            Product_{j=0..n+1, j<>n} (u+j-1) du, i.e. (n+1)!*lcm(1..n+2)
 *            times the Lagrangian coefficient of f(x_{-n}) in
 *            y(x_0)-y(x_{-1}) = h*Sum_{k=0..n+1} c_k f(x_{-k}) (equivalently
 *            a(n) = (-1)^n (n+1)! lcm(1..n+2) (g(n) + (n+1) g(n+1)), g =
 *            Adams-M...
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 8, i64 n <= 14, i128 n <= 26,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 100 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* returns p if m = p^e (e >= 1), else 0 */
static idx_t a002406_ppbase(idx_t m) {
    idx_t p = 0, d;
    if (m < 2) return 0;
    for (d = 2; d * d <= m; d++) {
        if (m % d == 0) { p = d; break; }
    }
    if (p == 0) return m;
    while (m % p == 0) m /= p;
    return (m == 1) ? p : 0;
}

seq_t a(idx_t n) {
    idx_t len, j, k, i, m;
    seq_t *c = mem_new(seq_t, (size_t)(n + 2));
    seq_t L, S;

    /* build polynomial Prod_{j=0..n+1, j != n} (u + j - 1) */
    c[0] = 1;
    len = 1;
    for (j = 0; j <= n + 1; j++) {
        seq_t s;
        if (j == n) continue;
        s = (seq_t)j - 1;
        c[len] = c[len - 1];
        for (k = len - 1; k > 0; k--) {
            c[k] = c[k - 1] + s * c[k];
        }
        c[0] = s * c[0];
        len++;
    }

    /* L = lcm(1..n+2) */
    L = 1;
    for (m = 2; m <= n + 2; m++) {
        idx_t p = a002406_ppbase(m);
        if (p) L *= (seq_t)p;
    }

    /* L * integral over [0,1] */
    S = 0;
    for (i = 0; i < len; i++) {
        S += c[i] * (L / ((seq_t)i + 1));
    }
    mem_free(c);

    S = S * ((seq_t)n + 1);
    if (n & 1) S = -S;
    return S;
}
