/*
 * A004710  Positions of ones in binary expansion of Euler's constant gamma.
 * https://oeis.org/A004710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   binary digits of gamma via Brent-McMillan: gamma = U/V - log(N),
 *            N = 2^m, V = Sum_{k>=0} (N^k/k!)^2, U = Sum_{k>=0} (N^k/k!)^2 *
 *            H_k, log(2) from atanh series; a(n) = position of the n-th 1 bit
 *            after the binary point (search over computed bits)
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   add_to, addmul, bitlen_arr, cmp_arr, is_zero_arr, setbit,
 *            shl_bits, shr_bits, sub_from
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/add_to.h"
#include "../utils/addmul.h"
#include "../utils/bitlen_arr.h"
#include "../utils/cmp_arr.h"
#include "../utils/is_zero_arr.h"
#include "../utils/setbit.h"
#include "../utils/shl_bits.h"
#include "../utils/shr_bits.h"
#include "../utils/sub_from.h"

static uint64_t *g_pos = NULL;
static size_t g_cnt = 0;
static size_t g_lastP = 0;

static void divsmall(uint32_t *x, size_t L, uint32_t d)
{
    uint64_t r = 0;
    size_t i;
    for (i = L; i-- > 0;) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void div_ksq(uint32_t *x, size_t L, uint64_t k)
{
    if (k < 65536) divsmall(x, L, (uint32_t)(k * k));
    else { divsmall(x, L, (uint32_t)k); divsmall(x, L, (uint32_t)k); }
}

static void compute(size_t P)
{
    size_t G = P + 64;
    unsigned m = 1;
    size_t N, F, eb, L, L2, LQ, i, bv;
    uint64_t k;
    unsigned q0 = 0, j;
    uint32_t *A, *B, *U, *V, *Q, *T, *S, *tmp, *R;

    while (((size_t)1 << m) < G * 6932 / 40000 + 2) m++;
    N = (size_t)1 << m;
    F = G + 96;
    eb = 2 * N * 14427 / 10000 + 2;
    L = (F + eb + 2 * (m + 3) + 96) / 32 + 2;

    A = (uint32_t *)calloc(L, sizeof(uint32_t));
    B = (uint32_t *)calloc(L, sizeof(uint32_t));
    U = (uint32_t *)calloc(L, sizeof(uint32_t));
    V = (uint32_t *)calloc(L, sizeof(uint32_t));
    setbit(B, F);
    memcpy(V, B, L * sizeof(uint32_t));

    for (k = 1;; k++) {
        shl_bits(B, L, 2 * m);
        div_ksq(B, L, k);
        shl_bits(A, L, 2 * m);
        addmul(A, B, L, (uint32_t)k);
        div_ksq(A, L, k);
        add_to(U, A, L);
        add_to(V, B, L);
        if (k >= 2 * (uint64_t)N) {
            size_t ba = bitlen_arr(A, L);
            if (ba == 0 || bitlen_arr(V, L) > ba + G + 80) break;
        }
    }

    bv = bitlen_arr(V, L);
    if (bv > G + 64) {
        shr_bits(U, L, bv - (G + 64));
        shr_bits(V, L, bv - (G + 64));
    }
    L2 = (G + 64) / 32 + 3;
    if (L2 > L) L2 = L;

    while (cmp_arr(U, V, L2) >= 0) { sub_from(U, V, L2); q0++; }

    LQ = G / 32 + 3;
    Q = (uint32_t *)calloc(LQ, sizeof(uint32_t));
    for (i = 1; i <= G; i++) {
        shl_bits(U, L2, 1);
        if (cmp_arr(U, V, L2) >= 0) {
            sub_from(U, V, L2);
            setbit(Q, G - i);
        }
    }
    for (j = 0; (q0 >> j) != 0; j++)
        if ((q0 >> j) & 1) setbit(Q, G + j);

    /* log 2 = Sum_{k>=0} 2 / ((2k+1) 3^(2k+1)) */
    T = (uint32_t *)calloc(LQ, sizeof(uint32_t));
    S = (uint32_t *)calloc(LQ, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(LQ, sizeof(uint32_t));
    R = (uint32_t *)calloc(LQ, sizeof(uint32_t));
    setbit(T, G + 1);
    divsmall(T, LQ, 3);
    memcpy(S, T, LQ * sizeof(uint32_t));
    for (k = 1;; k++) {
        divsmall(T, LQ, 9);
        if (is_zero_arr(T, LQ)) break;
        memcpy(tmp, T, LQ * sizeof(uint32_t));
        divsmall(tmp, LQ, (uint32_t)(2 * k + 1));
        add_to(S, tmp, LQ);
    }
    addmul(R, S, LQ, m);
    sub_from(Q, R, LQ);

    free(g_pos);
    g_pos = (uint64_t *)malloc((P + 1) * sizeof(uint64_t));
    g_cnt = 0;
    for (i = 1; i <= P; i++) {
        size_t bp = G - i;
        if ((Q[bp / 32] >> (bp % 32)) & 1) g_pos[g_cnt++] = (uint64_t)i;
    }
    g_lastP = P;

    free(A); free(B); free(U); free(V);
    free(Q); free(T); free(S); free(tmp); free(R);
}

seq_t a(idx_t n)
{
    while (g_cnt < n) {
        size_t P = (size_t)(2 * n + n / 8 + 256);
        if (P < g_lastP * 3 / 2) P = g_lastP * 3 / 2;
        compute(P);
    }
    return (seq_t)g_pos[n - 1];
}
