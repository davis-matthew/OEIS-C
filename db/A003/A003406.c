/*
 * A003406  Expansion of Ramanujan's function R(x) = 1 + Sum_{n >= 1} {
 *          x^(n*(n+1)/2) / ((1+x)(1+x^2)(1+x^3)...(1+x^n)) }.
 * https://oeis.org/A003406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f. R(x) = 1 + Sum_{k>=1} x^(k(k+1)/2) / Product_{j=1..k}
 *            (1+x^j), evaluated in nested form R = 1 + x/(1+x)*(1 +
 *            x^2/(1+x^2)*(1 + x^3/(1+x^3)*(...))). Series are truncated at
 *            x^N and computed modulo 2^64; since |a(n)| is tiny (bounded by a
 *            divisor count), the result is exact
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *a003406_tab = NULL;
static idx_t a003406_cap = 0; /* number of valid entries */

static void a003406_build(idx_t N) {
    uint64_t *f = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    uint64_t *g = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    idx_t K = 0, i, k;
    f[0] = 1;
    while ((K + 1) * (K + 2) / 2 <= N) K++;
    for (k = K; k >= 1; k--) {
        uint64_t *t;
        /* g = x^k * f / (1 + x^k), coefficients computed in increasing order */
        for (i = 0; i < k && i <= N; i++) g[i] = 0;
        for (i = k; i <= N; i++) g[i] = f[i - k] - g[i - k];
        g[0] += 1; /* 1 + ... */
        t = f; f = g; g = t;
    }
    free(g);
    free(a003406_tab);
    a003406_tab = f;
    a003406_cap = N + 1;
}

seq_t a(idx_t n) {
    if (n >= a003406_cap) {
        idx_t N = a003406_cap * 2;
        if (N < 1024) N = 1024;
        if (N < n) N = n;
        a003406_build(N);
    }
    return (seq_t)(int64_t)a003406_tab[n];
}
