/*
 * A007260  McKay-Thompson series of class 6a for Monster.
 * https://oeis.org/A007260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] ( E(x)^6/E(x^3)^6 - 27*x*E(x^3)^6/E(x)^6 ), E(x) =
 *            prod_{k>=1} (1-x^k), i.e. expansion of q*((eta(q^2)/eta(q^6))^6
 *            - 27*(eta(q^6)/eta(q^2))^6) in powers of q^2. E(x)^3 =
 *            Sum_{m>=0} (-1)^m (2m+1) x^(m(m+1)/2) (Jacobi), so everything is
 *            done with sparse products and sparse series divis...
 * Range:     i8 n <= 1, i16 n <= 6, i32 n <= 23, i64 n <= 87, i128 n <= 330,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *T = 0;
static idx_t cap = 0;

static idx_t *je = 0;
static long long *jc = 0;
static idx_t nj = 0;

/* dst = src / J(x^step), where J(x) = E(x)^3 (sparse, J_0 = 1) */
static void sdiv(seq_t *dst, const seq_t *src, idx_t step, idx_t L)
{
    idx_t n, i;
    for (n = 0; n < L; n++) {
        seq_t s = src[n];
        for (i = 1; i < nj; i++) {
            idx_t e = je[i] * step;
            if (e > n) break;
            s -= dst[n - e] * jc[i];
        }
        dst[n] = s;
    }
}

/* dst = J(x^step)^2 truncated to L terms */
static void ssq(seq_t *dst, idx_t step, idx_t L)
{
    idx_t i, j;
    for (i = 0; i < nj; i++) {
        idx_t ei = je[i] * step;
        if (ei >= L) break;
        for (j = 0; j < nj; j++) {
            idx_t e = ei + je[j] * step;
            seq_t c;
            if (e >= L) break;
            c = jc[i];
            dst[e] += c * jc[j];
        }
    }
}

static void build(idx_t L)
{
    seq_t *S1, *S3, *Q, *A, *B;
    idx_t m, i;

    free(je);
    free(jc);
    nj = 0;
    for (m = 0; m * (m + 1) / 2 < L; m++) nj++;
    je = (idx_t *)malloc(nj * sizeof(idx_t));
    jc = (long long *)malloc(nj * sizeof(long long));
    for (m = 0; m < nj; m++) {
        je[m] = m * (m + 1) / 2;
        jc[m] = (m % 2 ? -1LL : 1LL) * (long long)(2 * m + 1);
    }

    S1 = alloc_zero(L);
    S3 = alloc_zero(L);
    Q = alloc_zero(L);
    A = alloc_zero(L);
    B = alloc_zero(L);

    ssq(S1, 1, L);          /* E(x)^6   */
    ssq(S3, 3, L);          /* E(x^3)^6 */

    sdiv(Q, S1, 3, L);
    sdiv(A, Q, 3, L);       /* A = E(x)^6 / E(x^3)^6 */

    sdiv(Q, S3, 1, L);
    sdiv(B, Q, 1, L);       /* B = E(x^3)^6 / E(x)^6 */

    if (T) release(T, cap);
    T = alloc_zero(L);
    T[0] = A[0];
    for (i = 1; i < L; i++)
        T[i] = A[i] - 27 * B[i - 1];
    cap = L;

    release(S1, L);
    release(S3, L);
    release(Q, L);
    release(A, L);
    release(B, L);
}

seq_t a(idx_t n)
{
    if (n >= cap) {
        idx_t L = cap + cap / 2;
        if (L < n + 1) L = n + 1;
        if (L < 64) L = 64;
        build(L);
    }
    return T[n];
}
