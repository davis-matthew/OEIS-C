/*
 * A007863  Number of hybrid binary trees with n internal nodes.
 * https://oeis.org/A007863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. satisfies A(x) = 1 + x*A(x) + x*A(x)^2 + x^2*A(x)^3, so
 *            a(n) = a(n-1) + [x^(n-1)]A^2 + [x^(n-2)]A^3, a(0) = 1
 *            (fixed-width builds); a(n) = (1/(n+1))*Sum_{k=0..n}
 *            binomial(n+k,n)*binomial(n+k+1,n-k) (big builds)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 909 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t a_conv(idx_t n) {
    seq_t *A, *A2, *A3, r;
    idx_t m, i;
    if (n == 0) return 1;
    A = alloc_zero(n + 1);
    A2 = alloc_zero(n + 1);
    A3 = alloc_zero(n + 1);
    A[0] = 1;
    for (m = 1; m <= n; m++) {
        seq_t s = 0;
        for (i = 0; i <= m - 1; i++) s += A[i] * A[m - 1 - i];
        A2[m - 1] = s;
        A[m] = A[m - 1] + A2[m - 1];
        if (m >= 2) {
            seq_t t = 0;
            for (i = 0; i <= m - 2; i++) t += A[i] * A2[m - 2 - i];
            A3[m - 2] = t;
            A[m] += A3[m - 2];
        }
    }
    r = A[n];
    release(A, n + 1);
    release(A2, n + 1);
    release(A3, n + 1);
    return r;
}

static seq_t a_sum(idx_t n) {
    seq_t sum = 0;
    seq_t b1 = 1;                 /* binomial(n+k, n) */
    seq_t b2 = (seq_t)n + 1;      /* binomial(n+k+1, n-k) */
    idx_t k;
    for (k = 0; k <= n; k++) {
        sum += b1 * b2;
        if (k < n) {
            b1 = b1 * (seq_t)(n + k + 1) / (seq_t)(k + 1);
            b2 = b2 * (seq_t)(n + k + 2) * (seq_t)(n - k)
                 / ((seq_t)(2 * k + 2) * (seq_t)(2 * k + 3));
        }
    }
    return sum / ((seq_t)n + 1);
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_conv(n);
    return a_sum(n);
}
