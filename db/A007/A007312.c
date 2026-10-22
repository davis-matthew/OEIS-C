/*
 * A007312  Reversion of g.f. (with constant term omitted) for partition
 *          numbers.
 * https://oeis.org/A007312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/n) * [x^(n-1)] G(x)^(-n), where G(x) = Sum_{j>=0}
 *            p(j+1) x^j and p = A000041 (Lagrange inversion); h = G^(-n) via
 *            h(0) = 1, h(k) = (1/k) * Sum_{j=1..k} ((1-n)*j - k) * p(j+1) *
 *            h(k-j)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 16, i64 n <= 30, i128 n <= 56,
 *            big any n
 * Helpers:   a_i128, alloc_zero, from_i128, parts_i128, parts_seq, release,
 *            seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/a_i128.h"
#include "../utils/parts_i128.h"
#include "../utils/parts_seq.h"

static seq_t a_seq(idx_t n) {
    seq_t *p = alloc_zero(n + 1);
    seq_t *h = alloc_zero(n);
    parts_seq(p, n);
    seq_t nn = (seq_t)n;
    h[0] = 1;
    for (idx_t k = 1; k < n; k++) {
        seq_t s = 0;
        seq_t kk = (seq_t)k;
        for (idx_t j = 1; j <= k; j++) {
            seq_t c = (seq_t)j * (1 - nn) - kk;
            s += (c * p[j + 1]) * h[k - j];
        }
        h[k] = s / kk;
    }
    seq_t r = h[n - 1] / nn;
    release(p, n + 1);
    release(h, n);
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (seq_is_fixed_width()) return from_i128(a_i128(n));
    return a_seq(n);
}
