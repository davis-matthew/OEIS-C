/*
 * A002284  q-expansion of modular form of weight 13/2: eta(8 tau)^12 *
 *          theta(tau).
 * https://oeis.org/A002284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(n) = Sum_{m in Z} c((n-4-m^2)/8), where c(k) = [x^k]
 *            Product_{k>0} (1-x^k)^12 (eta(8 tau)^12 * theta(tau)); eta^12 =
 *            (eta^3)^4 with eta^3 = Sum_{k>=0} (-1)^k (2k+1) x^(k(k+1)/2)
 * Range:     i8 n <= 31, i16 n <= 140, i32 n <= 1004, i64 n <= 1004,
 *            i128 n <= 1004, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 4) return 0;
    idx_t m4 = n - 4;
    idx_t N = m4 / 8;
    long long *g = (long long *)calloc(N + 1, sizeof(long long));
    long long *g2 = (long long *)calloc(N + 1, sizeof(long long));
    long long *f = (long long *)calloc(N + 1, sizeof(long long));
    idx_t *nz = (idx_t *)malloc((N + 2) * sizeof(idx_t));
    idx_t cnt = 0, i, j, k;
    long long total = 0;
    if (!g || !g2 || !f || !nz) {
        free(g); free(g2); free(f); free(nz);
        return 0;
    }
    for (k = 0; k * (k + 1) / 2 <= N; k++) {
        idx_t t = k * (k + 1) / 2;
        g[t] = (k & 1) ? -(long long)(2 * k + 1) : (long long)(2 * k + 1);
        nz[cnt++] = t;
    }
    for (i = 0; i < cnt; i++)
        for (j = 0; j < cnt; j++) {
            idx_t s = nz[i] + nz[j];
            if (s <= N) g2[s] += g[nz[i]] * g[nz[j]];
        }
    for (i = 0; i <= N; i++) {
        long long s = 0;
        for (j = 0; j <= i; j++) s += g2[j] * g2[i - j];
        f[i] = s;
    }
    for (k = 0; k * k <= m4; k++) {
        idx_t r = m4 - k * k;
        if (r % 8 == 0) total += (k == 0 ? 1 : 2) * f[r / 8];
    }
    free(g); free(g2); free(f); free(nz);
    return (seq_t)total;
}
