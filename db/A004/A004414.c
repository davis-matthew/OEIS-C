/*
 * A004414  Expansion of (Sum_{n=-inf..inf} x^(n^2))^(-13).
 * https://oeis.org/A004414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. 1/theta_3(x)^13. Arbitrary precision: J.C.P. Miller power
 *            recurrence a(0) = 1, a(n) = (1/n) * Sum_{j>=1, j^2<=n}
 *            2*(-12*j^2 - n)*a(n-j^2). Fixed width: a(n) = (-1)^n * [x^n]
 *            Product_{k>=1} ((1+x^k)/(1-x^k))^13 (all partial coefficients
 *            nonnegative)
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 10, i64 n <= 31, i128 n <= 98,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static seq_t big_a(idx_t n) {
    if (n + 1 > memo_len) {
        idx_t newlen = n + 1;
        if (memo == NULL) {
            memo = alloc_zero(newlen);
        } else {
            memo = grow_zero(memo, memo_len, newlen);
        }
        idx_t start = memo_len;
        if (start == 0) {
            memo[0] = 1;
            start = 1;
        }
        for (idx_t m = start; m <= n; m++) {
            seq_t s = 0;
            for (idx_t j = 1; j * j <= m; j++) {
                seq_t c = (seq_t)j * (seq_t)j * (-12) - (seq_t)m;
                s += c * 2 * memo[m - j * j];
            }
            memo[m] = s / (seq_t)m;
        }
        memo_len = newlen;
    }
    return memo[n];
}

static seq_t fixed_a(idx_t n) {
    seq_t *f = alloc_zero(n + 1);
    f[0] = 1;
    for (idx_t k = 1; k <= n; k++) {
        for (int r = 0; r < 13; r++) {
            for (idx_t i = n; i >= k; i--) {
                f[i] += f[i - k];
                if (i == k) break;
            }
        }
        for (int r = 0; r < 13; r++) {
            for (idx_t i = k; i <= n; i++) {
                f[i] += f[i - k];
            }
        }
    }
    seq_t v = f[n];
    release(f, n + 1);
    if (n % 2 == 1) v = -v;
    return v;
}

seq_t a(idx_t n) {
    if (seq_is_big()) return big_a(n);
    return fixed_a(n);
}
