/*
 * A000950  Number of forests with n nodes and height at most 3.
 * https://oeis.org/A000950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f.: exp(x*exp(x*exp(x*exp(x)))). With T1(m) = m (e.g.f.
 *            x*exp(x)), for each level: F(0) = 1, F(m) = Sum_{k=1..m}
 *            binomial(m-1,k-1)*T(k)*F(m-k) [F = exp(T)], then the next level
 *            is T'(m) = m*F(m-1) [T' = x*exp(T)]. a(n) = F3(n), where F3 =
 *            exp(T3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a000950_cache = NULL;
static idx_t a000950_cap = 0;

static void a000950_build(idx_t N) {
    seq_t *t = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    seq_t *f = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    seq_t *bin = (seq_t *)malloc((size_t)(N + 2) * sizeof(seq_t));
    idx_t m, k, j;
    int level;

    for (m = 0; m <= N; m++) {
        t[m] = (seq_t)m;
        f[m] = 0;
    }
    for (level = 0; level < 3; level++) {
        /* f = exp(t), exponential-coefficient form */
        f[0] = 1;
        for (j = 0; j <= N + 1; j++) bin[j] = 0;
        bin[0] = 1; /* row 0 of Pascal's triangle */
        for (m = 1; m <= N; m++) {
            /* bin holds row m-1 */
            seq_t sum = 0;
            for (k = 1; k <= m; k++) {
                sum += bin[k - 1] * t[k] * f[m - k];
            }
            f[m] = sum;
            /* advance bin to row m */
            for (j = m; j >= 1; j--) bin[j] += bin[j - 1];
        }
        if (level < 2) {
            t[0] = 0;
            for (m = 1; m <= N; m++) t[m] = (seq_t)m * f[m - 1];
        }
    }

    free(t);
    free(bin);
    if (a000950_cache) free(a000950_cache);
    a000950_cache = f;
    a000950_cap = N;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (a000950_cache == NULL || n > a000950_cap) {
        a000950_build(n);
    }
    return a000950_cache[n];
}
