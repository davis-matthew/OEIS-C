/*
 * A000594  Ramanujan's tau function (or Ramanujan numbers, or tau numbers).
 * https://oeis.org/A000594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, core, mult, nice
 * Formula:   G.f.: x * Product_{k>=1} (1 - x^k)^24 = x * (eta^3)^8, where
 *            Product_{k>=1} (1 - x^k)^3 = Sum_{m>=0} (-1)^m (2m+1)
 *            x^(m(m+1)/2) (Jacobi). a(n) = coefficient of x^(n-1) in
 *            (Sum_{m>=0} (-1)^m (2m+1) x^(m(m+1)/2))^8
 * Range:     i8 n <= 2, i16 n <= 7, i32 n <= 46, i64 n <= 2562,
 *            i128 n <= 9968, big any n
 * Helpers:   alloc_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"

static seq_t *a594_T = 0;
static idx_t a594_cap = 0;

static void a594_build(idx_t N) {
    seq_t *R = alloc_zero(N);
    seq_t *S = alloc_zero(N);
    idx_t cnt = 0, m, i, k, step;
    idx_t *tri, *wt;
    int *neg;
    while (cnt * (cnt + 1) / 2 < N) cnt++;
    tri = (idx_t *)malloc(sizeof(idx_t) * (cnt + 1));
    wt = (idx_t *)malloc(sizeof(idx_t) * (cnt + 1));
    neg = (int *)malloc(sizeof(int) * (cnt + 1));
    for (m = 0; m < cnt; m++) {
        tri[m] = m * (m + 1) / 2;
        wt[m] = 2 * m + 1;
        neg[m] = (int)(m & 1);
        if (neg[m]) R[tri[m]] -= wt[m];
        else R[tri[m]] += wt[m];
    }
    for (step = 0; step < 7; step++) {
        for (i = 0; i < N; i++) S[i] = 0;
        for (i = 0; i < N; i++) {
            if (R[i] == 0) continue;
            for (k = 0; k < cnt && i + tri[k] < N; k++) {
                if (neg[k]) S[i + tri[k]] -= R[i] * wt[k];
                else S[i + tri[k]] += R[i] * wt[k];
            }
        }
        { seq_t *t = R; R = S; S = t; }
    }
    free(tri);
    free(wt);
    free(neg);
    a594_T = R;
    a594_cap = N;
}

seq_t a(idx_t n) {
    if (n > a594_cap) {
        idx_t N = 2 * a594_cap;
        if (N < 64) N = 64;
        if (N < n) N = n;
        a594_build(N);
    }
    return a594_T[n - 1];
}
