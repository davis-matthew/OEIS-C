/*
 * A002286  Bisection of A002470.
 * https://oeis.org/A002286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A002470(2n-1), where A002470 is the q-expansion of
 *            theta_3(q)^4 * F(q), F(q) = eta(q)^4*eta(q^2)^2*eta(q^4)^4 =
 *            Sum_{m>=1} f(m) q^m with f(m) = (1/4) * Sum_{x^2+y^2=m} (x^4 -
 *            6x^2y^2 + y^4), and theta_3(q)^4 = Sum r4(k) q^k, r4(0) = 1,
 *            r4(k) = 8 * Sum_{d|k, 4 does not divide d} d. So a(n)...
 * Range:     i8 n <= 4, i16 n <= 25, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   from_i128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/from_i128.h"

static long long *F5 = 0;
static long long *R4 = 0;
static idx_t cap = 0;

static void build(idx_t M)
{
    long long *f = (long long *)malloc((size_t)(M + 1) * sizeof(long long));
    long long *r = (long long *)malloc((size_t)(M + 1) * sizeof(long long));
    idx_t d, k;
    long long x, y;
    memset(f, 0, (size_t)(M + 1) * sizeof(long long));
    memset(r, 0, (size_t)(M + 1) * sizeof(long long));
    for (d = 1; d <= M; d++) {
        if (d % 4 == 0) continue;
        for (k = d; k <= M; k += d) r[k] += 8 * (long long)d;
    }
    r[0] = 1;
    for (x = 0; (idx_t)(x * x) <= M; x++) {
        for (y = 0; (idx_t)(x * x + y * y) <= M; y++) {
            long long x2 = x * x, y2 = y * y;
            long long v = x2 * x2 - 6 * x2 * y2 + y2 * y2;
            long long mult = (x == 0 ? 1 : 2) * (y == 0 ? 1 : 2);
            f[x2 + y2] += v * mult;
        }
    }
    f[0] = 0;
    for (k = 1; k <= M; k++) f[k] /= 4;
    free(F5);
    free(R4);
    F5 = f;
    R4 = r;
    cap = M;
}

seq_t a(idx_t n)
{
    idx_t m = 2 * n - 1;
    idx_t j;
    __int128 s = 0;
    if (m > cap) {
        idx_t M = m;
        if (M < 2 * cap) M = 2 * cap;
        if (M < 64) M = 64;
        build(M);
    }
    for (j = 1; j <= m; j++) {
        if (F5[j] != 0) s += (__int128)F5[j] * (__int128)R4[m - j];
    }
    return from_i128(s);
}
