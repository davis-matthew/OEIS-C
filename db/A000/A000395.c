/*
 * A000395  6th power of rooted tree enumerator; number of linear forests of 6
 *          rooted trees.
 * https://oeis.org/A000395
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] B(x)^6, where B(x) = Sum_{k>=1} b(k) x^k is the
 *            g.f. of A000081: b(1) = 1, b(k+1) = (1/k) * Sum_{j=1..k} s(j) *
 *            b(k-j+1), s(j) = Sum_{d|j} d*b(d). With C(x) = B(x)/x: a(n) =
 *            Sum_{i=0..n-6} C3[i]*C3[n-6-i], C3 = C^3 (computed as (C*C)*C)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 24, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   a306_alloc0, a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_alloc0.h"
#include "../utils/a306_grow.h"

static seq_t *tb = NULL;   /* tb[k] = A000081(k), k = 1..bn */
static seq_t *ts = NULL;   /* ts[k] = sum_{d|k} d*tb[d] */
static seq_t *C2 = NULL;   /* coefficients of C^2, indices 0..ccap-1 */
static seq_t *C3 = NULL;   /* coefficients of C^3 */
static idx_t bn = 0, bcap = 0, ccap = 0;

static void compute_s(idx_t k) {
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= k; d++) {
        if (k % d == 0) {
            idx_t e = k / d;
            sum += (seq_t)d * tb[d];
            if (e != d) sum += (seq_t)e * tb[e];
        }
    }
    ts[k] = sum;
}

static void extend(idx_t m) {
    /* need C indices 0..m, i.e. b indices 1..m+1 */
    idx_t need_b = m + 1;
    if (tb == NULL) {
        bcap = need_b + 1;
        tb = a306_alloc0(bcap + 1);
        ts = a306_alloc0(bcap + 1);
        tb[1] = 1;
        bn = 1;
        compute_s(1);
    } else if (need_b + 1 > bcap) {
        idx_t nc = need_b + 1;
        tb = a306_grow(tb, bcap + 1, nc + 1);
        ts = a306_grow(ts, bcap + 1, nc + 1);
        bcap = nc;
    }
    while (bn < need_b) {
        idx_t k = bn;
        seq_t sum = 0;
        for (idx_t j = 1; j <= k; j++)
            sum += ts[j] * tb[k - j + 1];
        tb[k + 1] = sum / (seq_t)k;
        bn = k + 1;
        compute_s(bn);
    }
    if (m + 1 > ccap) {
        idx_t newlen = m + 1;
        if (C2 == NULL) {
            C2 = a306_alloc0(newlen);
            C3 = a306_alloc0(newlen);
        } else {
            C2 = a306_grow(C2, ccap, newlen);
            C3 = a306_grow(C3, ccap, newlen);
        }
        for (idx_t i = ccap; i < newlen; i++) {
            seq_t s2 = 0;
            for (idx_t j = 0; j <= i; j++)
                s2 += tb[j + 1] * tb[i - j + 1];
            C2[i] = s2;
            seq_t s3 = 0;
            for (idx_t j = 0; j <= i; j++)
                s3 += C2[j] * tb[i - j + 1];
            C3[i] = s3;
        }
        ccap = newlen;
    }
}

seq_t a(idx_t n) {
    if (n < 6) return 0;
    idx_t m = n - 6;
    extend(m);
    seq_t r = 0;
    for (idx_t i = 0; i <= m; i++)
        r += C3[i] * C3[m - i];
    return r;
}
