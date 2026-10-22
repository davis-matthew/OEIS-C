/*
 * A003782  Coefficients of Jacobi Eisenstein series of index 1 and weight 6.
 * https://oeis.org/A003782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. t3^3*(t3^8 - 11*t3^4*t2^4/8 + 11*t2^8/32), with t3 = Sum_k
 *            q^(k^2) and t2 = Sum_k q^((k+1/2)^2) = 2*q^(1/4)*psi(q^2),
 *            psi(x) = Sum_{m>=0} x^(m(m+1)/2). Since t2^4 = 16*q*psi(q^2)^4,
 *            with P = psi(q^2)^4 this is a(n) = [q^n] t3^11 - 22*[q^(n-1)]
 *            t3^7*P + 88*[q^(n-2)] t3^3*P^2
 * Range:     i8 n <= 3, i16 n <= 11, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   make_series, mul_psi, mul_t3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/make_series.h"
#include "../utils/mul_psi.h"
#include "../utils/mul_t3.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void build(idx_t L) {
    seq_t *A = make_series(L, 0, 11);
    seq_t *B = make_series(L, 4, 7);
    seq_t *C = make_series(L, 8, 3);
    seq_t *r = (seq_t *)malloc(L * sizeof(seq_t));
    for (idx_t i = 0; i < L; i++) {
        seq_t v = A[i];
        if (i >= 1) v -= 22 * B[i - 1];
        if (i >= 2) v += 88 * C[i - 2];
        r[i] = v;
    }
    free(A); free(B); free(C);
    free(cache);
    cache = r;
    cache_len = L;
}

seq_t a(idx_t n) {
    if (n >= cache_len) {
        idx_t L = 2 * cache_len;
        if (L < n + 1) L = n + 1;
        if (L < 64) L = 64;
        build(L);
    }
    return cache[n];
}
