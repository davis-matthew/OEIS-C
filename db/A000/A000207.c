/*
 * A000207  Number of inequivalent ways of dissecting a regular (n+2)-gon into
 *          n triangles by n-1 non-intersecting diagonals under rotations and
 *          reflections; also the number of (unlabeled) maximal outerplanar
 *          graphs on n+2 vertices.
 * https://oeis.org/A000207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = C(N)/(2*N) + C(N/2+1)/4 + C(k)/2 + C(N/3+1)/3 with N =
 *            n+2, where C(m) = A000108(m-2) if m is an integer, 0 otherwise,
 *            and k = (N+1)/2 if N is odd, k = N/2+1 if N is even. Computed
 *            exactly as 12*N*a(n) = 6*C(N) + N*(3*C(N/2+1) + 6*C(k) +
 *            4*C(N/3+1)) using a small multi-limb integer, then d...
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 22, u64 n <= 39, u128 n <= 72,
 *            big any n
 * Helpers:   lm_add, lm_cat, lm_div, lm_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/lm_add.h"
#include "../utils/lm_cat.h"
#include "../utils/lm_div.h"
#include "../utils/lm_mul.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n + 2;
    size_t L = (size_t)((2 * (uint64_t)n + 96) / 32 + 2);
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *U = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint64_t m;
    seq_t res = 0;
    size_t i;

    /* 6*C(N) = 6*Catalan(n) */
    lm_cat(S, L, n);
    lm_mul(S, L, 6);

    /* 3*C(N/2+1) */
    if (N % 2 == 0) {
        lm_cat(U, L, N / 2 - 1);
        lm_mul(U, L, 3);
        lm_add(T, U, L);
    }
    /* 6*C(k) */
    m = (N % 2) ? (N - 3) / 2 : N / 2 - 1;
    lm_cat(U, L, m);
    lm_mul(U, L, 6);
    lm_add(T, U, L);
    /* 4*C(N/3+1) */
    if (N % 3 == 0) {
        lm_cat(U, L, N / 3 - 1);
        lm_mul(U, L, 4);
        lm_add(T, U, L);
    }

    lm_mul(T, L, N);
    lm_add(S, T, L);
    lm_div(S, L, 12 * N);

    i = L;
    while (i-- > 0) {
        res = res * 65536u + (seq_t)(S[i] >> 16);
        res = res * 65536u + (seq_t)(S[i] & 0xFFFFu);
    }

    free(S);
    free(T);
    free(U);
    return res;
}
