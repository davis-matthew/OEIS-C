/*
 * A003784  Coefficients of Jacobi cusp form of index 1 and weight 10.
 * https://oeis.org/A003784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. = eta(q^4)^18 * theta_4(q), i.e. a(n) = Sum_{s>=0, 4 |
 *            n-3-s^2} c(s) * [x^((n-3-s^2)/4)] Product_{k>0} (1 - x^k)^18,
 *            with c(0) = 1, c(s) = 2*(-1)^s for s > 0; Product (1-x^k)^3 =
 *            Sum_{m>=0} (-1)^m (2m+1) x^(m(m+1)/2)
 * Range:     i8 n <= 11, i16 n <= 39, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t r = n - 3;
    idx_t N = r / 4;

    /* sparse terms of Product (1 - x^k)^3 up to x^N */
    idx_t cnt = 0;
    while ((cnt * (cnt + 1)) / 2 <= N) cnt++;
    idx_t *ex = (idx_t *)malloc(cnt * sizeof(idx_t));
    long long *co = (long long *)malloc(cnt * sizeof(long long));
    for (idx_t m = 0; m < cnt; m++) {
        ex[m] = (m * (m + 1)) / 2;
        co[m] = (m % 2 ? -1LL : 1LL) * (long long)(2 * m + 1);
    }

    seq_t *P = alloc_zero(N + 1);
    seq_t *Q = alloc_zero(N + 1);
    for (idx_t m = 0; m < cnt; m++) P[ex[m]] = (seq_t)co[m];

    for (int t = 0; t < 5; t++) {
        for (idx_t j = 0; j <= N; j++) {
            seq_t s = 0;
            for (idx_t m = 0; m < cnt && ex[m] <= j; m++)
                s += (seq_t)co[m] * P[j - ex[m]];
            Q[j] = s;
        }
        seq_t *tmp = P; P = Q; Q = tmp;
    }
    /* P now holds Product (1 - x^k)^18 up to x^N */

    seq_t result = 0;
    for (idx_t s = 0; s * s <= r; s++) {
        idx_t rem = r - s * s;
        if (rem % 4 != 0) continue;
        seq_t c = (s == 0) ? (seq_t)1 : (seq_t)((s % 2) ? -2 : 2);
        result += c * P[rem / 4];
    }

    free(ex); free(co);
    release(P, N + 1);
    release(Q, N + 1);
    return result;
}
