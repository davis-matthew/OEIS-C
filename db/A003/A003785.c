/*
 * A003785  Coefficients of Jacobi cusp form of index 1 and weight 12.
 * https://oeis.org/A003785
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   G.f. = (theta_3(q)^4 + theta_2(q)^4/4) * eta(4z)^18 * theta_4(q)
 *            = q^3 * (theta_3(q)^4 + 4*q*psi(q^2)^4) * (prod_{k>=1}
 *            (1-q^{4k})^3)^6 * theta_4(q), where prod (1-x^k)^3 = Sum_{j>=0}
 *            (-1)^j (2j+1) x^{j(j+1)/2}, theta_3 = Sum q^{k^2}, theta_4 = Sum
 *            (-1)^k q^{k^2}, psi(x) = Sum_{k>=0} x^{k(k+1)/2}
 * Range:     i8 n <= 7, i16 n <= 23, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   alloc_zero, mulsp, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/mulsp.h"

static seq_t *cache = 0;
static idx_t cache_len = 0;

static void build(idx_t L)
{
    seq_t *S = alloc_zero(L);
    seq_t *X = alloc_zero(L);
    seq_t *Y = alloc_zero(L);
    seq_t *R = alloc_zero(L);
    idx_t i;
    int r;
    S[0] = 1;
    for (r = 0; r < 6; r++) mulsp(S, L, 0);
    mulsp(S, L, 2);
    for (i = 0; i < L; i++) { X[i] = S[i]; Y[i] = S[i]; }
    for (r = 0; r < 4; r++) mulsp(X, L, 1);
    for (r = 0; r < 4; r++) mulsp(Y, L, 3);
    for (i = 0; i < L; i++) {
        R[i] = X[i];
        if (i >= 1) R[i] += Y[i - 1] * 4;
    }
    release(S, L);
    release(X, L);
    release(Y, L);
    if (cache) release(cache, cache_len);
    cache = R;
    cache_len = L;
}

seq_t a(idx_t n)
{
    idx_t m;
    if (n < 3) return 0;
    m = n - 3;
    if (m >= cache_len) {
        idx_t L = cache_len * 2;
        if (L < m + 1) L = m + 1;
        if (L < 64) L = 64;
        build(L);
    }
    return cache[m];
}
