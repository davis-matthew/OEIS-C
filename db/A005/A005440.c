/*
 * A005440  Coefficients of Gandhi polynomials.
 * https://oeis.org/A005440
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A_n'(1), where the Gandhi polynomials are A_1(x) = 1,
 *            A_{n+1}(x) = x^2*A_n(x+1) - (x-1)^2*A_n(x). Writing D_n = A_n',
 *            D_{n+1}(k) = k^2*D_n(k+1) + 2k*A_n(k+1) - (k-1)^2*D_n(k) -
 *            2(k-1)*A_n(k), with A_1(k) = 1 and D_1(k) = 0; a(n) = D_n(1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *memo = 0;
static idx_t memo_len = 0;

static void compute(idx_t N)
{
    /* levels m = 1..N-1; level m needs k = 1..N+2-m */
    idx_t K = N + 2;
    seq_t *A = alloc_zero(K + 1);
    seq_t *D = alloc_zero(K + 1);
    idx_t k, m;
    if (memo) release(memo, memo_len);
    memo = alloc_zero(N);
    memo_len = N;
    for (k = 1; k <= K; k++) {
        A[k] = 1;
        D[k] = 0;
    }
    memo[1] = D[1];
    for (m = 1; m + 1 < N; m++) {
        idx_t top = K - m; /* new level m+1 valid for k = 1..K-m */
        for (k = 1; k <= top; k++) {
            idx_t k2 = k * k, j2 = (k - 1) * (k - 1);
            idx_t k1 = 2 * k, j1 = 2 * (k - 1);
            seq_t nd = D[k + 1] * k2;
            seq_t sub, na, s2;
            nd += A[k + 1] * k1;
            sub = D[k] * j2;
            sub += A[k] * j1;
            nd -= sub;
            na = A[k + 1] * k2;
            s2 = A[k] * j2;
            na -= s2;
            A[k] = na;
            D[k] = nd;
        }
        memo[m + 1] = D[1];
    }
    release(A, K + 1);
    release(D, K + 1);
}

seq_t a(idx_t n)
{
    if (n >= memo_len) {
        idx_t N = n + 1;
        if (N < 2 * memo_len) N = 2 * memo_len;
        if (N < 4) N = 4;
        compute(N);
    }
    return memo[n];
}
