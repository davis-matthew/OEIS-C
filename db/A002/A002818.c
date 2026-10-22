/*
 * A002818  Nearest integer to exp n^2.
 * https://oeis.org/A002818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(exp(n^2)). e is computed as sum 1/k! in binary
 *            fixed point, then raised to the n^2-th power by repeated
 *            squaring. The fixed point carries enough guard bits that the
 *            result rounds exactly
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   a2818_add, a2818_divsmall, a2818_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2818_add.h"
#include "../utils/a2818_divsmall.h"
#include "../utils/a2818_mul.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n * (uint64_t)n;
    size_t B, F, L, lg = 0, i, q, r, h;
    uint32_t *X, *R, *T, *tmp;
    uint64_t e;
    uint32_t k;
    seq_t v = 0;

    if (N == 0) return 1;
    while (lg < 63 && ((uint64_t)1 << lg) <= N) lg++;
    B = (size_t)((double)N * 1.4426950408889634) + 2;
    F = B + 2 * lg + 96;
    L = (F + B) / 32 + 4;

    X = (uint32_t *)calloc(L, sizeof(uint32_t));
    R = (uint32_t *)calloc(L, sizeof(uint32_t));
    T = (uint32_t *)calloc(L, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(2 * L, sizeof(uint32_t));

    /* X = e * 2^F */
    T[F / 32] = (uint32_t)1 << (F % 32);
    memcpy(X, T, sizeof(uint32_t) * L);
    for (k = 1;; k++) {
        if (!a2818_divsmall(T, L, k)) break;
        a2818_add(X, T, L);
    }

    /* R = e^N * 2^F */
    R[F / 32] = (uint32_t)1 << (F % 32);
    e = N;
    for (;;) {
        if (e & 1) a2818_mul(R, X, R, tmp, L, F);
        e >>= 1;
        if (!e) break;
        a2818_mul(X, X, X, tmp, L, F);
    }

    /* round: add 2^(F-1), shift right by F */
    memset(T, 0, sizeof(uint32_t) * L);
    T[(F - 1) / 32] = (uint32_t)1 << ((F - 1) % 32);
    a2818_add(R, T, L);
    q = F / 32;
    r = F % 32;
    for (i = 0; i < L; i++) {
        uint32_t lo = (i + q < L) ? R[i + q] : 0;
        uint32_t hi = (i + q + 1 < L) ? R[i + q + 1] : 0;
        T[i] = r ? ((lo >> r) | (hi << (32 - r))) : lo;
    }

    h = L;
    while (h > 0 && T[h - 1] == 0) h--;
    while (h-- > 0) {
        int b;
        for (b = 3; b >= 0; b--) {
            v = v * 256 + (seq_t)((T[h] >> (8 * b)) & 255u);
        }
    }

    free(X);
    free(R);
    free(T);
    free(tmp);
    return v;
}
