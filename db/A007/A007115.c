/*
 * A007115  Expansion of e.g.f. (1 - x - x^2)^x.
 * https://oeis.org/A007115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   E.g.f. A(x) = exp(G(x)), G(x) = x*log(1-x-x^2) = Sum_{k>=2} c(k)
 *            x^k/k!, c(k) = -k*(k-2)!*Lucas(k-1), c(1) = 0, and a(n+1) =
 *            Sum_{k=0..n} binomial(n,k)*c(k+1)*a(n-k), a(0) = 1
 * Range:     i8 n <= 7, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 31,
 *            big any n
 * Helpers:   alloc_zero, from_i128, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/from_i128.h"
#include "../utils/seq_is_fixed_width.h"

typedef unsigned __int128 u128;

/* Fixed-width builds: exact arithmetic modulo 2^128 (wrapping), result fits. */
static u128 *fx = NULL;
static idx_t fx_len = 0;

static void fill_fixed(idx_t n) {
    if (n < fx_len) return;
    idx_t len = n + 1;
    u128 *A = (u128 *)malloc((size_t)len * sizeof(u128));
    u128 *C = (u128 *)malloc((size_t)(len + 1) * sizeof(u128));
    u128 *R = (u128 *)malloc((size_t)(len + 1) * sizeof(u128));
    u128 fact = 1, Lp = 2, L = 1;
    C[0] = 0;
    C[1] = 0;
    for (idx_t k = 2; k <= n; k++) {
        C[k] = (u128)0 - (u128)k * fact * L;
        fact *= (u128)(k - 1);
        u128 t = L + Lp;
        Lp = L;
        L = t;
    }
    A[0] = 1;
    R[0] = 1;
    for (idx_t m = 0; m + 1 <= n; m++) {
        u128 s = 0;
        for (idx_t k = 1; k <= m; k++)
            s += R[k] * C[k + 1] * A[m - k];
        s += C[m + 1] * 0; /* placeholder no-op for clarity */
        A[m + 1] = s + R[0] * C[1] * A[m];
        /* advance Pascal row from m to m+1 */
        R[m + 1] = 1;
        for (idx_t k = m; k >= 1; k--)
            R[k] += R[k - 1];
    }
    free(fx);
    fx = A;
    fx_len = len;
    free(C);
    free(R);
}

/* Arbitrary-precision build. */
static seq_t *bx = NULL;
static idx_t bx_len = 0;

static void fill_big(idx_t n) {
    if (n < bx_len) return;
    idx_t len = n + 1;
    seq_t *A = alloc_zero(len);
    seq_t *C = alloc_zero(len + 1);
    seq_t *R = alloc_zero(len + 1);
    seq_t fact = 1, Lp = 2, L = 1;
    for (idx_t k = 2; k <= n; k++) {
        C[k] = (seq_t)0 - (seq_t)k * fact * L;
        fact *= (seq_t)(k - 1);
        seq_t t = L + Lp;
        Lp = L;
        L = t;
    }
    A[0] = 1;
    R[0] = 1;
    for (idx_t m = 0; m + 1 <= n; m++) {
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++)
            s += R[k] * C[k + 1] * A[m - k];
        A[m + 1] = s;
        R[m + 1] = 1;
        for (idx_t k = m; k >= 1; k--)
            R[k] += R[k - 1];
    }
    if (bx) release(bx, bx_len);
    bx = A;
    bx_len = len;
    release(C, len + 1);
    release(R, len + 1);
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) {
        fill_fixed(n);
        return from_i128((__int128)fx[n]);
    }
    fill_big(n);
    return bx[n];
}
