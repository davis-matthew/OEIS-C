/*
 * A000728  Expansion of Product_{n>=1} (1-x^n)^5.
 * https://oeis.org/A000728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Product_{n>=1} (1-x^n)^5 = [Product (1-x^n)]^2 * [Product
 *            (1-x^n)]^3, with Product (1-x^n) = Sum_{k} (-1)^k x^(k(3k-1)/2)
 *            (Euler) and Product (1-x^n)^3 = Sum_{k>=0} (-1)^k (2k+1)
 *            x^(k(k+1)/2) (Jacobi), so a(n) = Sum_{k>=0, k(k+1)/2<=n} (-1)^k
 *            (2k+1) * E2(n - k(k+1)/2), where E2 = coefficients of...
 * Range:     i8 n <= 60, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a728_e2 = NULL;
static idx_t a728_len = 0;

static void a728_ensure(idx_t n) {
    idx_t N, m, k;
    int *E;
    if (n < a728_len) return;
    N = n + 1;
    if (N < 2 * a728_len) N = 2 * a728_len;
    if (N < 64) N = 64;
    E = (int *)calloc((size_t)N, sizeof(int));
    E[0] = 1;
    for (k = 1;; k++) {
        idx_t g1 = k * (3 * k - 1) / 2, g2;
        int s = (k & 1) ? -1 : 1;
        if (g1 >= N) break;
        E[g1] = s;
        g2 = g1 + k;
        if (g2 < N) E[g2] = s;
    }
    free(a728_e2);
    a728_e2 = (seq_t *)malloc((size_t)N * sizeof(seq_t));
    for (m = 0; m < N; m++) {
        seq_t sum = 0;
        sum += (seq_t)E[m];
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2, g2;
            int s = (k & 1) ? -1 : 1;
            if (g1 > m) break;
            sum += (seq_t)(s * E[m - g1]);
            g2 = g1 + k;
            if (g2 <= m) sum += (seq_t)(s * E[m - g2]);
        }
        a728_e2[m] = sum;
    }
    free(E);
    a728_len = N;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t k;
    a728_ensure(n);
    for (k = 0; k * (k + 1) / 2 <= n; k++) {
        idx_t T = k * (k + 1) / 2;
        long long c = (long long)(2 * k + 1);
        if (k & 1) c = -c;
        sum += (seq_t)c * a728_e2[n - T];
    }
    return sum;
}
