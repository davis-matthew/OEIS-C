/*
 * A001482  Expansion of (Product_{j>=1} (1-(-x)^j) - 1)^4 in powers of x.
 * https://oeis.org/A001482
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] (P(x) - 1)^4, where P(x) = Product_{j>=1}
 *            (1-(-x)^j) = Sum_{k} (-1)^k (-1)^m x^m, m = k*(3*k-1)/2 (Euler's
 *            pentagonal number theorem, applied to x -> -x)
 * Range:     i8 n <= 170, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    long long N = (long long)n;
    long long *pos;
    int *sgn;
    long long *q2;
    long long cnt = 0, cap = 16, k, i, j;
    seq_t result = 0;

    if (n < 4) return 0;

    pos = (long long *)malloc(sizeof(long long) * (size_t)cap);
    sgn = (int *)malloc(sizeof(int) * (size_t)cap);

    /* nonzero coefficients of q(x) = P(x) - 1 up to degree N */
    for (k = 1;; k++) {
        long long m1 = k * (3 * k - 1) / 2;
        long long m2 = k * (3 * k + 1) / 2;
        int sk = (k % 2 == 0) ? 1 : -1;
        if (m1 > N) break;
        if (cnt + 2 > cap) {
            cap *= 2;
            pos = (long long *)realloc(pos, sizeof(long long) * (size_t)cap);
            sgn = (int *)realloc(sgn, sizeof(int) * (size_t)cap);
        }
        pos[cnt] = m1;
        sgn[cnt] = sk * ((m1 % 2 == 0) ? 1 : -1);
        cnt++;
        if (m2 <= N) {
            pos[cnt] = m2;
            sgn[cnt] = sk * ((m2 % 2 == 0) ? 1 : -1);
            cnt++;
        }
    }

    /* q^2 coefficients: small integers (bounded by number of pairs) */
    q2 = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    for (i = 0; i < cnt; i++) {
        for (j = 0; j < cnt; j++) {
            long long d = pos[i] + pos[j];
            if (d <= N) q2[d] += (long long)(sgn[i] * sgn[j]);
        }
    }

    /* q^4 at n */
    for (i = 0; i <= N; i++) {
        if (q2[i] != 0 && q2[N - i] != 0) {
            result += (seq_t)q2[i] * (seq_t)q2[N - i];
        }
    }

    free(pos);
    free(sgn);
    free(q2);
    return result;
}
