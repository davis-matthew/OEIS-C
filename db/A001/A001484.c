/*
 * A001484  Expansion of (Product_{j>=1} (1-(-x)^j) - 1)^6 in powers of x.
 * https://oeis.org/A001484
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] (QPochhammer(-x) - 1)^6, where QPochhammer(x) =
 *            Sum_{k in Z} (-1)^k x^(k(3k-1)/2) (pentagonal number theorem);
 *            computed as g = (QPochhammer(-x)-1)^3 by sparse convolution,
 *            then a(n) = Sum_{i} g(i)*g(n-i)
 * Range:     i8 n <= 16, i16 n <= 2145, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 6) return (seq_t)0;
    long long N = (long long)n;
    long long L = N + 1;

    /* sparse nonzero terms of f(x) = QPochhammer(-x) - 1 */
    long long cap = 16;
    while ((cap / 2) * (cap / 2) < 4 * L) cap *= 2;
    cap = cap * 2 + 16;
    long long *pos = (long long *)malloc((size_t)cap * sizeof(long long));
    long long *sg = (long long *)malloc((size_t)cap * sizeof(long long));
    long long cnt = 0;
    for (long long k = 1;; k++) {
        long long m1 = k * (3 * k - 1) / 2;
        long long m2 = k * (3 * k + 1) / 2;
        if (m1 > N) break;
        long long s = (k % 2 == 0) ? 1 : -1;
        long long s1 = s * ((m1 % 2 == 0) ? 1 : -1);
        pos[cnt] = m1; sg[cnt] = s1; cnt++;
        if (m2 <= N) {
            long long s2 = s * ((m2 % 2 == 0) ? 1 : -1);
            pos[cnt] = m2; sg[cnt] = s2; cnt++;
        }
    }

    long long *f = (long long *)calloc((size_t)L, sizeof(long long));
    long long *f2 = (long long *)calloc((size_t)L, sizeof(long long));
    long long *g = (long long *)calloc((size_t)L, sizeof(long long));
    for (long long t = 0; t < cnt; t++) f[pos[t]] = sg[t];

    /* f2 = f * f */
    for (long long t = 0; t < cnt; t++) {
        long long p = pos[t], s = sg[t];
        for (long long u = 0; u < cnt; u++) {
            long long q = p + pos[u];
            if (q <= N) f2[q] += s * sg[u];
        }
    }
    /* g = f2 * f */
    for (long long t = 0; t < cnt; t++) {
        long long p = pos[t], s = sg[t];
        for (long long i = 0; i + p <= N; i++) {
            if (f2[i]) g[i + p] += s * f2[i];
        }
    }
    /* a(n) = sum g[i] * g[n-i] */
    long long acc = 0;
    for (long long i = 0; i <= N; i++) {
        if (g[i] && g[N - i]) acc += g[i] * g[N - i];
    }

    free(pos); free(sg); free(f); free(f2); free(g);
    return (seq_t)acc;
}
