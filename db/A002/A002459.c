/*
 * A002459  Nearest integer to cosh(n).
 * https://oeis.org/A002459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = round(cosh(n)) = floor((e^n + e^(-n))/2 + 1/2), with e
 *            and 1/e computed exactly enough in fixed-point big arithmetic
 *            (sum of 1/k!, then binary powering)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   c2459_add, c2459_divs, c2459_iszero, c2459_len, c2459_mulfx,
 *            c2459_pow, c2459_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/c2459_add.h"
#include "../utils/c2459_divs.h"
#include "../utils/c2459_iszero.h"
#include "../utils/c2459_len.h"
#include "../utils/c2459_mulfx.h"
#include "../utils/c2459_pow.h"
#include "../utils/c2459_sub.h"

seq_t a(idx_t n) {
    uint64_t ib = (uint64_t)n * 1443 / 1000 + 2;
    size_t FL = (size_t)((ib + 96) / 32 + 1);
    size_t IL = (size_t)(ib / 32 + 2);
    size_t L = FL + IL;
    size_t i;
    uint32_t k;
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *E = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *Ei = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *R1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *R2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
    seq_t r = 0;

    /* e = sum 1/k!, 1/e = sum (-1)^k/k! */
    T[FL] = 1;
    memcpy(P, T, sizeof(uint32_t) * L);
    for (k = 1;; k++) {
        c2459_divs(T, L, k);
        if (c2459_iszero(T, L)) break;
        if (k & 1) c2459_add(N, T, L);
        else c2459_add(P, T, L);
    }
    memcpy(E, P, sizeof(uint32_t) * L);
    c2459_add(E, N, L);
    memcpy(Ei, P, sizeof(uint32_t) * L);
    c2459_sub(Ei, N, L);

    c2459_pow(R1, E, n, L, FL, B, tmp);
    c2459_pow(R2, Ei, n, L, FL, B, tmp);

    /* W = e^n + e^-n + 1, then floor(W / 2) */
    c2459_add(R1, R2, L);
    memset(T, 0, sizeof(uint32_t) * L);
    T[FL] = 1;
    c2459_add(R1, T, L);
    for (i = 0; i < L; i++) {
        uint32_t hi = (i + 1 < L) ? R1[i + 1] : 0;
        R1[i] = (R1[i] >> 1) | (hi << 31);
    }

    i = L;
    while (i-- > FL) {
        uint32_t w = R1[i];
        r = r * 256 + ((w >> 24) & 0xFF);
        r = r * 256 + ((w >> 16) & 0xFF);
        r = r * 256 + ((w >> 8) & 0xFF);
        r = r * 256 + (w & 0xFF);
    }

    free(T); free(P); free(N); free(E); free(Ei);
    free(R1); free(R2); free(B); free(tmp);
    return r;
}
