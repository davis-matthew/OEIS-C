/*
 * A002399  Coefficients for step-by-step integration.
 * https://oeis.org/A002399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * lcm(1..n+1) * Integral_{s=0..1} s*Product_{j=2..n}
 *            (s+j) ds (the negated Adams-Bashforth weight of f(x_{-1}) for
 *            the (n+1)-step formula, scaled by n!*lcm(1..n+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   a002399_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a002399_g.h"

seq_t a(idx_t n) {
    idx_t m = n + 1, k, j;
    seq_t *c = (seq_t *)calloc((size_t)(n + 2), sizeof(seq_t));
    seq_t sum = 0, L = 1, res;
    uint64_t L64 = 1;
    int small = (m <= 42);

    for (k = 0; k <= n + 1; k++) c[k] = 0;
    c[1] = 1;                       /* P(s) = s */
    for (j = 2; j <= n; j++) {      /* P(s) *= (s + j) */
        idx_t d = j - 1;            /* current degree */
        for (k = d + 1; k >= 1; k--)
            c[k] = c[k - 1] + c[k] * (seq_t)j;
        c[0] = c[0] * (seq_t)j;
    }

    if (small) {
        for (k = 2; k <= m; k++) L64 = L64 / a002399_g(L64, k) * k;
    } else {
        idx_t p;
        for (p = 2; p <= m; p++) {
            idx_t q, pe;
            int isp = 1;
            for (q = 2; q * q <= p; q++) if (p % q == 0) { isp = 0; break; }
            if (!isp) continue;
            pe = p;
            while (pe <= m / p) pe *= p;
            L *= (seq_t)pe;
        }
    }

    for (k = 1; k <= n; k++) {
        seq_t w;
        if (small) w = (seq_t)(L64 / (uint64_t)(k + 1));
        else w = L / (seq_t)(k + 1);
        sum += c[k] * w;
    }
    res = (seq_t)n * sum;
    free(c);
    return res;
}
