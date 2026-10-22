/*
 * A001455  Number of permutations of length n with longest increasing
 *          subsequence of length 4.
 * https://oeis.org/A001455
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = u4(n) - u3(n), where u_k(n) = #permutations with LIS <=
 *            k. u3(n) = Sum_{k=0..n} C(2k,k)*C(n+1,k+1)*C(n+2,k+1) /
 *            ((n+1)^2*(n+2)) (Gessel). u4(n) = n!^2 * [x^(2n)]
 *            det[I_{i-j}(2x)]_{i,j=1..4} (Gessel's Bessel determinant),
 *            expanded as Sum_sigma sgn(sigma) * n!^2 * [y^M] g_a g_b g_c g_d
 *            with...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 40,
 *            big any n
 * Helpers:   a1455_binom, a1455_isfixed, a1455_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a1455_binom.h"
#include "../utils/a1455_isfixed.h"
#include "../utils/a1455_small.h"

/* multisets {a,b,c,d} of |i - sigma(i)| over S4, with summed signs */
static const int a1455_cls[12][5] = {
    {0, 0, 0, 0, 1}, {0, 0, 1, 1, -3}, {0, 1, 1, 2, 4}, {0, 0, 2, 2, -2},
    {1, 1, 1, 1, 1}, {1, 1, 1, 3, -2}, {1, 1, 2, 2, -2}, {0, 1, 2, 3, 4},
    {0, 0, 3, 3, -1}, {2, 2, 2, 2, 1}, {1, 2, 2, 3, -2}, {1, 1, 3, 3, 1}
};

static seq_t a1455_big(idx_t n) {
    seq_t pos = 0, neg = 0, Q, u4, u3, S3, t;
    idx_t k;
    int c;
    for (c = 0; c < 12; c++) {
        idx_t a = (idx_t)a1455_cls[c][0], b = (idx_t)a1455_cls[c][1];
        idx_t cc = (idx_t)a1455_cls[c][2], d = (idx_t)a1455_cls[c][3];
        int coef = a1455_cls[c][4];
        idx_t D = a + b + cc + d, h = D / 2, s1 = a + b, s2 = cc + d, M, Nh, i;
        seq_t S, F;
        if (n < h) continue;
        M = n - h;
        Nh = n + h;
        t = a1455_binom(s1, a) * a1455_binom(2 * M + s2, M + cc) * a1455_binom(Nh, s1);
        S = t;
        for (k = 0; k < M; k++) {
            idx_t m = M - k;
            uint64_t num1 = (uint64_t)(2 * k + s1 + 1) * (2 * k + s1 + 2) * (m + cc) * (m + d);
            uint64_t num2 = (uint64_t)(M - k) * (Nh - k - s1);
            uint64_t den1 = (uint64_t)(k + a + 1) * (k + b + 1) * (2 * m + s2) * (2 * m + s2 - 1);
            uint64_t den2 = (uint64_t)(k + 1) * (k + s1 + 1);
            t = t * (seq_t)num1;
            t = t * (seq_t)num2;
            t = t / (seq_t)den1;
            t = t / (seq_t)den2;
            S += t;
        }
        F = 1;
        for (i = M + 1; i <= n; i++) F = F * (seq_t)i;
        for (i = Nh + 1; i <= n + 4; i++) F = F * (seq_t)i;
        S = S * F;
        if (coef > 0) pos += S * (seq_t)(idx_t)coef;
        else neg += S * (seq_t)(idx_t)(-coef);
    }
    Q = (seq_t)(n + 1) * (seq_t)(n + 2) * (seq_t)(n + 3) * (seq_t)(n + 4);
    u4 = (pos - neg) / Q;

    t = (seq_t)(n + 1) * (seq_t)(n + 2);
    S3 = t;
    for (k = 0; k < n; k++) {
        uint64_t num = (uint64_t)2 * (2 * k + 1) * (n - k) * (n + 1 - k);
        uint64_t den = (uint64_t)(k + 1) * (k + 2) * (k + 2);
        t = t * (seq_t)num;
        t = t / (seq_t)den;
        S3 += t;
    }
    u3 = S3 / ((seq_t)(n + 1) * (seq_t)(n + 1) * (seq_t)(n + 2));
    return u4 - u3;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if (a1455_isfixed()) return a1455_small(n);
    return a1455_big(n);
}
