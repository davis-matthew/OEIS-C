/*
 * A002044  MacMahon's solid partitions of n in which 3 is the smallest
 *          summand.
 * https://oeis.org/A002044
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{i>=3} (-1*[i=3] + 1/(1-x^i)^binomial(i+1,2)),
 *            i.e. a(n) = Sum_{k>=1} binomial(k+5,5) * P(n-3k), where P(x) =
 *            Product_{i>=4} 1/(1-x^i)^binomial(i+1,2)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   a2044_big, a2044_g, a2044_next, alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/a2044_big.h"
#include "../utils/a2044_g.h"
#include "../utils/a2044_next.h"

static seq_t *a2044_P = 0;
static idx_t a2044_cap = 0;

static void a2044_build(idx_t N) {
    seq_t *P = alloc_zero(N + 1);
    idx_t i, j, m, k, d;
    P[0] = 1;
    if (a2044_big()) {
        seq_t *s = alloc_zero(N + 1);
        for (d = 4; d <= N; d++) {
            seq_t v = (seq_t)d * ((seq_t)d + 1) / 2 * d;
            for (k = d; k <= N; k += d) s[k] += v;
        }
        for (m = 1; m <= N; m++) {
            seq_t acc = 0;
            for (k = 4; k <= m; k++) acc += s[k] * P[m - k];
            P[m] = acc / m;
        }
    } else {
        seq_t *coef = alloc_zero(N / 4 + 2);
        for (i = 4; i <= N; i++) {
            idx_t c = i * (i + 1) / 2;
            idx_t M = N / i;
            coef[0] = 1;
            for (m = 1; m <= M; m++)
                coef[m] = a2044_next(coef[m - 1], c + m - 1, m);
            for (j = N; j >= i; j--) {
                seq_t acc = P[j];
                for (m = 1; m * i <= j; m++) acc += P[j - m * i] * coef[m];
                P[j] = acc;
            }
        }
    }
    a2044_P = P;
    a2044_cap = N;
}

seq_t a(idx_t n) {
    seq_t res = 0, c = 1;
    idx_t k;
    if (n < 3) return 0;
    if (a2044_P == 0 || n > a2044_cap) {
        idx_t N = a2044_cap + a2044_cap / 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        a2044_build(N);
    }
    for (k = 1; 3 * k <= n; k++) {
        c = a2044_next(c, k + 5, k);
        res += c * a2044_P[n - 3 * k];
    }
    return res;
}
