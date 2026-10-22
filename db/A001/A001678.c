/*
 * A001678  Number of series-reduced planted trees with n nodes.
 * https://oeis.org/A001678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   for n >= 3, a(n) = r(n-1), where r(n) counts rooted trees with
 *            no node of out-degree 1, and E(x) = Sum e(n) x^n =
 *            Product_{k>=1} 1/(1-x^k)^r(k). r(1) = 1, and r(n) = e(n-1) -
 *            r(n-1) for n >= 2. e(0) = 1, n*e(n) = Sum_{k=1..n} c(k)*e(n-k)
 *            with c(k) = Sum_{d|k} d*r(d) (this is the Euler transform)....
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 37, u64 n <= 66, u128 n <= 124,
 *            big any n
 * Helpers:   fixed_width_mode
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fixed_width_mode.h"

static seq_t *R = NULL;   /* R[1..have] */
static idx_t have = 0;

static void build(idx_t N) {
    seq_t *r = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (fixed_width_mode()) {
        /* No division by n: multiply in one factor 1/(1-x^k)^r(k) at a time. */
        seq_t *F = (seq_t *)calloc(N + 1, sizeof(seq_t));
        seq_t *C = (seq_t *)calloc(N + 1, sizeof(seq_t));
        F[0] = 1;
        for (idx_t j = 1; j <= N; j++) {
            r[j] = F[j - 1];
            idx_t k = j - 1;
            if (k >= 1 && r[k] != 0 && N - 1 >= k) {
                seq_t rk = r[k];
                idx_t tmax = (N - 1) / k;
                C[0] = 1;
                for (idx_t t = 1; t <= tmax; t++)
                    C[t] = C[t - 1] * (rk + (seq_t)(t - 1)) / (seq_t)t;
                for (idx_t m = N - 1; m >= k; m--) {
                    seq_t s = 0;
                    for (idx_t t = 1; t * k <= m; t++)
                        s += C[t] * F[m - t * k];
                    F[m] += s;
                }
            }
        }
        free(F);
        free(C);
    } else {
        seq_t *e = (seq_t *)calloc(N + 1, sizeof(seq_t));
        seq_t *c = (seq_t *)calloc(N + 1, sizeof(seq_t));
        e[0] = 1;
        r[1] = 1;
        for (idx_t n = 1; n <= N; n++) {
            if (n >= 2) r[n] = e[n - 1] - r[n - 1];
            if (n == N) break;
            seq_t cn = 0;
            for (idx_t d = 1; d * d <= n; d++) {
                if (n % d == 0) {
                    cn += (seq_t)d * r[d];
                    idx_t q = n / d;
                    if (q != d) cn += (seq_t)q * r[q];
                }
            }
            c[n] = cn;
            seq_t s = 0;
            for (idx_t k = 1; k <= n; k++)
                s += c[k] * e[n - k];
            e[n] = s / (seq_t)n;
        }
        free(e);
        free(c);
    }
    if (R) free(R);
    R = r;
    have = N;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    idx_t need = n - 1;
    if (need > have) {
        idx_t N = need;
        if (N < 2 * have) N = 2 * have;
        if (N < 64) N = 64;
        build(N);
    }
    return R[need];
}
