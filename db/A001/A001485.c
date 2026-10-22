/*
 * A001485  Expansion of (Product_{j>=1} (1-(-x)^j) - 1)^7 in powers of x.
 * https://oeis.org/A001485
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] ( QPochhammer(-x) - 1 )^7, where QPochhammer(-x) -
 *            1 = Sum_{k>=1} (-1)^(k+m) x^m over generalized pentagonal m =
 *            k(3k-1)/2, k(3k+1)/2 (Euler's pentagonal theorem); the series is
 *            raised to the 7th power by repeated sparse multiplication
 * Range:     i8 n <= 13, i16 n <= 399, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 7) return 0;
    idx_t N = n + 1;
    idx_t cap = 16;
    idx_t cnt = 0;
    idx_t *ex = (idx_t *)malloc(cap * sizeof(idx_t));
    int *sg = (int *)malloc(cap * sizeof(int));
    for (idx_t k = 1;; k++) {
        idx_t m1 = k * (3 * k - 1) / 2;
        if (m1 > n) break;
        idx_t m2 = k * (3 * k + 1) / 2;
        idx_t ms[2];
        ms[0] = m1;
        ms[1] = m2;
        for (int t = 0; t < 2; t++) {
            idx_t m = ms[t];
            if (m > n) continue;
            if (cnt == cap) {
                cap *= 2;
                ex = (idx_t *)realloc(ex, cap * sizeof(idx_t));
                sg = (int *)realloc(sg, cap * sizeof(int));
            }
            ex[cnt] = m;
            sg[cnt] = (((k & 1) ^ (m & 1)) != 0) ? -1 : 1;
            cnt++;
        }
    }
    long long *res = (long long *)malloc(N * sizeof(long long));
    long long *tmp = (long long *)malloc(N * sizeof(long long));
    for (idx_t i = 0; i < N; i++) res[i] = 0;
    for (idx_t j = 0; j < cnt; j++) res[ex[j]] += sg[j];
    for (int pw = 1; pw < 7; pw++) {
        for (idx_t i = 0; i < N; i++) tmp[i] = 0;
        for (idx_t i = 0; i < N; i++) {
            long long r = res[i];
            if (r == 0) continue;
            for (idx_t j = 0; j < cnt; j++) {
                idx_t e = ex[j];
                if (e > n - i) continue;
                if (sg[j] > 0) tmp[i + e] += r;
                else tmp[i + e] -= r;
            }
        }
        for (idx_t i = 0; i < N; i++) res[i] = tmp[i];
    }
    long long ans = res[n];
    free(ex);
    free(sg);
    free(res);
    free(tmp);
    return (seq_t)ans;
}
