/*
 * A000219  Number of plane partitions (or planar partitions) of n.
 * https://oeis.org/A000219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, core
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} a(n-k)*sigma_2(k), a(0) = 1; for
 *            fixed-width builds and small n: G.f. Product_{k>=1} 1/(1-x^k)^k
 *            (Euler transform, additions only)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 47, u64 n <= 121, u128 n <= 320,
 *            big any n
 * Helpers:   alloc_series, fixed_mode, sigma2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_series.h"
#include "../utils/fixed_mode.h"
#include "../utils/sigma2.h"

#define FIX_N 400

static seq_t F[FIX_N + 1];
static int F_done = 0;

static seq_t *A = NULL;
static uint64_t *S2 = NULL;
static size_t A_cnt = 0;
static size_t A_cap = 0;

static void grow(size_t need)
{
    size_t cap = A_cap ? A_cap : 64;
    size_t i;
    seq_t *nA;
    uint64_t *nS;
    while (cap < need) cap *= 2;
    nA = alloc_series(cap);
    for (i = 0; i < A_cnt; i++) nA[i] = A[i];
    nS = (uint64_t *)realloc(S2, cap * sizeof(uint64_t));
    for (i = A_cap; i < cap; i++) nS[i] = sigma2((uint64_t)i);
    A = nA;
    S2 = nS;
    A_cap = cap;
}

seq_t a(idx_t n)
{
    size_t m, k;
    if (n <= FIX_N && fixed_mode()) {
        if (!F_done) {
            size_t j, r;
            F[0] = 1;
            for (k = 1; k <= FIX_N; k++)
                for (r = 0; r < k; r++)
                    for (j = k; j <= FIX_N; j++)
                        F[j] += F[j - k];
            F_done = 1;
        }
        return F[n];
    }
    if ((size_t)n + 1 > A_cap) grow((size_t)n + 1);
    for (m = A_cnt; m <= (size_t)n; m++) {
        if (m == 0) {
            A[0] = 1;
        } else {
            seq_t s = 0;
            for (k = 1; k <= m; k++)
                s += A[m - k] * (seq_t)S2[k];
            A[m] = s / (seq_t)m;
        }
        A_cnt = m + 1;
    }
    return A[n];
}
