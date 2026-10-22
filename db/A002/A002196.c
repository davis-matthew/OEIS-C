/*
 * A002196  Denominators of coefficients for numerical integration.
 * https://oeis.org/A002196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (1/(2n+1)!) * Integral_{u=0..1}
 *            Product_{k=1..n} (u - k^2) du (substituting u = t^2 in
 *            (2/(2n+1)!)*Integral_{t=0..1} t*Product(t^2-k^2) dt). With
 *            Product_{k=1..n}(u + k^2) = Sum_j c_j u^j and L = lcm(1..n+1): N
 *            = Sum_j (-1)^(n-j) c_j * L/(j+1), and a(n) = denominator of N
 *            /...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   a2196_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a2196_gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* coefficients of Product_{k=1..n} (u + k^2) */
    seq_t *c = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    idx_t j, k;
    for (j = 0; j <= n; j++) c[j] = 0;
    c[0] = 1;
    for (k = 1; k <= n; k++) {
        seq_t k2 = (seq_t)k * (seq_t)k;
        for (j = k; j >= 1; j--) c[j] = c[j - 1] + k2 * c[j];
        c[0] = c[0] * k2;
    }

    /* L = lcm(1..n+1) */
    idx_t m = n + 1;
    bool *comp = (bool *)malloc((size_t)(m + 1) * sizeof(bool));
    for (j = 0; j <= m; j++) comp[j] = false;
    seq_t L = 1;
    for (idx_t p = 2; p <= m; p++) {
        if (comp[p]) continue;
        for (idx_t q = p * p; q <= m; q += p) comp[q] = true;
        idx_t pe = p;
        while (pe <= m / p) pe *= p;
        L *= (seq_t)pe;
    }
    free(comp);

    seq_t pos = 0, neg = 0;
    for (j = 0; j <= n; j++) {
        seq_t t = c[j] * (L / (seq_t)(j + 1));
        if (((n - j) & 1) == 0) pos += t;
        else neg += t;
    }
    free(c);
    seq_t N = (pos >= neg) ? (pos - neg) : (neg - pos);
    if (N == 0) return 1;

    seq_t F = 1;
    for (idx_t i = 2; i <= 2 * n + 1; i++) F *= (seq_t)i;

    seq_t g = a2196_gcd(N, F);
    seq_t N1 = N / g;
    seq_t F1 = F / g;
    seq_t g2 = a2196_gcd(N1, L);
    return F1 * (L / g2);
}
