/*
 * A000342  Number of n-node rooted trees of height 5.
 * https://oeis.org/A000342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001385(n) - A001384(n), where t_h(n) = number of n-node
 *            rooted trees of height <= h: t_0 = x, and sum t_h(n) x^n = x *
 *            prod_{s>=1} (1-x^s)^(-t_{h-1}(s)). The products are expanded
 *            division-free, using multichoose(c,m) = binom(c+m-1,m) built
 *            step by step with gcd cancellation
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   alloc_zero, gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/gcd_seq.h"

static seq_t *T[6];
static idx_t cap = 0;

static void build(idx_t N) {
    for (int h = 0; h <= 5; h++) {
        if (T[h]) free(T[h]);
        T[h] = alloc_zero(N + 1);
    }
    T[0][1] = 1;
    seq_t *P = alloc_zero(N);
    seq_t *mc = alloc_zero(N);
    for (int h = 1; h <= 5; h++) {
        for (idx_t j = 0; j < N; j++) P[j] = 0;
        P[0] = 1;
        for (idx_t s = 1; s < N; s++) {
            seq_t c = T[h - 1][s];
            if (c == 0) continue;
            idx_t M = (N - 1) / s;
            mc[0] = 1;
            for (idx_t i = 0; i < M; i++) {
                seq_t r = mc[i];
                seq_t d = (seq_t)(i + 1);
                seq_t g = gcd_seq(r % d, d);
                mc[i + 1] = (r / g) * ((c + (seq_t)i) / (d / g));
            }
            for (idx_t j = N - 1; j >= s; j--) {
                seq_t sum = P[j];
                idx_t m = 1;
                for (idx_t ms = s; ms <= j; ms += s, m++) {
                    sum += mc[m] * P[j - ms];
                }
                P[j] = sum;
                if (j == s) break;
            }
        }
        for (idx_t j = 1; j <= N; j++) T[h][j] = P[j - 1];
    }
    free(P);
    free(mc);
    cap = N;
}

seq_t a(idx_t n) {
    if (n <= 5) return 0;
    if (n > cap) {
        idx_t N = n;
        if (N < 64) N = 64;
        build(N);
    }
    return T[5][n] - T[4][n];
}
