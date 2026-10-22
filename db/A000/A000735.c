/*
 * A000735  Expansion of Product_{k>=1} (1 - x^k)^12.
 * https://oeis.org/A000735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f. Product_{k>=1} (1 - x^k)^12 = (eta^6)^2, where eta(q)^3 =
 *            Sum_{k>=0} (-1)^k (2k+1) x^(k(k+1)/2) (Jacobi); eta^6 is the
 *            square of that sparse series, and a(n) = Sum_{m=0..n} c6(m) *
 *            c6(n-m)
 * Range:     i8 n <= 4, i16 n <= 29, i32 n <= 1918, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"

static seq_t *e6 = 0;
static idx_t e6_len = 0;

static void build_e6(idx_t N) {
    seq_t *c = alloc_zero(N + 1);
    idx_t i, j;
    for (i = 0; i * (i + 1) / 2 <= N; i++) {
        idx_t ti = i * (i + 1) / 2;
        seq_t ci = (seq_t)(long long)(2 * i + 1);
        if (i & 1) ci = -ci;
        for (j = 0; ti + j * (j + 1) / 2 <= N; j++) {
            idx_t tj = j * (j + 1) / 2;
            seq_t cj = (seq_t)(long long)(2 * j + 1);
            if (j & 1) cj = -cj;
            c[ti + tj] += ci * cj;
        }
    }
    e6 = c;
    e6_len = N + 1;
}

seq_t a(idx_t n) {
    idx_t m;
    seq_t s = 0;
    if (n + 1 > e6_len) {
        idx_t N = n;
        if (N < 2 * e6_len) N = 2 * e6_len;
        if (N < 64) N = 64;
        build_e6(N);
    }
    for (m = 0; m <= n; m++)
        s += e6[m] * e6[n - m];
    return s;
}
