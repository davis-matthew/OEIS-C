/*
 * A001490  Expansion of {Product_{j>=1} (1 - (-x)^j) - 1}^12 in powers of x.
 * https://oeis.org/A001490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^(n-1)] Q(x)^12, where Q(x) = (Product_{j>=1} (1 -
 *            (-x)^j) - 1)/x and Product_{j>=1} (1 - (-x)^j) = Sum_{k} (-1)^k
 *            (-x)^(k(3k-1)/2) (pentagonal number theorem)
 * Range:     i8 n <= 3, i16 n <= 14, i32 n <= 1423, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A001490_CAP 65536

static seq_t a001490_R[A001490_CAP];

seq_t a(idx_t n) {
    if (n == 0 || n > A001490_CAP) return 0;
    long long N = (long long)(n - 1);
    static long long idx[2048];
    static int sgn[2048];
    int cnt = 0;
    long long k, i;
    int t, p;

    /* nonzero coefficients of Q (excluding Q[0] = 1) */
    for (k = 1;; k++) {
        long long m1 = k * (3 * k - 1) / 2;
        long long m2 = k * (3 * k + 1) / 2;
        if (m1 - 1 > N) break;
        if (m1 - 1 >= 1) {
            int s = ((k + m1) % 2 == 0) ? 1 : -1;
            idx[cnt] = m1 - 1; sgn[cnt] = s; cnt++;
        }
        if (m2 - 1 <= N) {
            int s = ((k + m2) % 2 == 0) ? 1 : -1;
            idx[cnt] = m2 - 1; sgn[cnt] = s; cnt++;
        }
    }

    for (i = 0; i <= N; i++) a001490_R[i] = 0;
    a001490_R[0] = 1;

    for (p = 0; p < 12; p++) {
        for (i = N; i >= 0; i--) {
            for (t = 0; t < cnt; t++) {
                long long d = idx[t];
                if (d > i) continue;
                if (sgn[t] > 0) a001490_R[i] += a001490_R[i - d];
                else a001490_R[i] -= a001490_R[i - d];
            }
        }
    }
    return a001490_R[N];
}
