/*
 * A005986  Number of column-strict plane partitions of n.
 * https://oeis.org/A005986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 1/Product_{i>=1} (1-x^i)^floor((i+3)/2). Arbitrary
 *            precision: a(0) = 1, a(n) = (1/n)*Sum_{k=1..n} b(k)*a(n-k),
 *            where b(k) = Sum_{d|k} d*floor((d+3)/2). Fixed width
 *            (division-free, exact modulo 2^w): multiply the series in turn
 *            by 1/(1-x^j)^m = Sum_t binomial(m+t-1,t) x^(j*t), with m =
 *            floor...
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 48, u64 n <= 133, u128 n <= 374,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *big_a = NULL;
static idx_t big_len = 0;

static void big_extend(idx_t need) {
    idx_t newlen = big_len * 2;
    if (newlen < need) newlen = need;
    if (newlen < 16) newlen = 16;
    idx_t *b = (idx_t *)calloc((size_t)newlen, sizeof(idx_t));
    for (idx_t d = 1; d < newlen; d++) {
        idx_t w = d * ((d + 3) / 2);
        for (idx_t k = d; k < newlen; k += d) b[k] += w;
    }
    big_a = grow_zero(big_a, big_len, newlen);
    idx_t start = big_len;
    if (start == 0) {
        big_a[0] = 1;
        start = 1;
    }
    for (idx_t i = start; i < newlen; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= i; k++) s += (seq_t)b[k] * big_a[i - k];
        big_a[i] = s / (seq_t)i;
    }
    big_len = newlen;
    free(b);
}

static seq_t *fix_a = NULL;
static idx_t fix_len = 0;

static void fix_compute(idx_t need) {
    idx_t N = fix_len * 2;
    if (N < need) N = need;
    if (N < 16) N = 16;
    seq_t *dp = alloc_zero(N);
    seq_t *c = alloc_zero(N);
    dp[0] = 1;
    for (idx_t j = 1; j < N; j++) {
        idx_t m = (j + 3) / 2;
        idx_t T = (N - 1) / j;
        for (idx_t t = 0; t <= T; t++) c[t] = 1;
        for (idx_t p = 1; p < m; p++)
            for (idx_t t = 1; t <= T; t++) c[t] += c[t - 1];
        for (idx_t k = N - 1; k >= j; k--) {
            seq_t s = dp[k];
            idx_t tmax = k / j;
            for (idx_t t = 1; t <= tmax; t++) s += c[t] * dp[k - j * t];
            dp[k] = s;
        }
    }
    release(c, N);
    if (fix_a != NULL) release(fix_a, fix_len);
    fix_a = dp;
    fix_len = N;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        if (n >= fix_len) fix_compute(n + 1);
        return fix_a[n];
    }
    if (n >= big_len) big_extend(n + 1);
    return big_a[n];
}
