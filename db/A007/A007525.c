/*
 * A007525  Decimal expansion of log_2 e.
 * https://oeis.org/A007525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th decimal digit of 1/log(2), with log(2) = Sum_{k>=0}
 *            2/((2k+1)*3^(2k+1)); computed as a big fixed-point value L =
 *            2^(32F)*log(2), then the digits of 2^(32F)/L by long division
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   a7525_add, a7525_cmp, a7525_divsmall, a7525_iszero, a7525_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a7525_add.h"
#include "../utils/a7525_cmp.h"
#include "../utils/a7525_divsmall.h"
#include "../utils/a7525_iszero.h"
#include "../utils/a7525_sub.h"

static unsigned char *a7525_dig = NULL;
static idx_t a7525_have = 0;

static void a7525_compute(idx_t D)
{
    size_t F = (size_t)(D / 9) + 4;
    size_t N = F + 1;
    uint32_t *t = (uint32_t *)calloc(N, sizeof(uint32_t));
    uint32_t *term = (uint32_t *)calloc(N, sizeof(uint32_t));
    uint32_t *L = (uint32_t *)calloc(N, sizeof(uint32_t));
    uint32_t *R = (uint32_t *)calloc(N, sizeof(uint32_t));
    uint64_t k = 0;
    idx_t i;

    /* t = 2 * 2^(32F) / 3 */
    t[F] = 2;
    a7525_divsmall(t, N, 3);
    while (!a7525_iszero(t, N)) {
        memcpy(term, t, N * sizeof(uint32_t));
        a7525_divsmall(term, N, (uint32_t)(2 * k + 1));
        a7525_add(L, term, N);
        a7525_divsmall(t, N, 9);
        k++;
    }

    /* R = 2^(32F) - L  (integer part of 2^(32F)/L is 1) */
    R[F] = 1;
    a7525_sub(R, L, N);

    free(a7525_dig);
    a7525_dig = (unsigned char *)malloc((size_t)D + 1);
    a7525_dig[1] = 1;
    for (i = 2; i <= D; i++) {
        uint64_t c = 0;
        size_t j;
        unsigned char d = 0;
        for (j = 0; j < N; j++) {
            uint64_t s = (uint64_t)R[j] * 10u + c;
            R[j] = (uint32_t)s;
            c = s >> 32;
        }
        while (a7525_cmp(R, L, N) >= 0) {
            a7525_sub(R, L, N);
            d++;
        }
        a7525_dig[i] = d;
    }
    a7525_have = D;

    free(t);
    free(term);
    free(L);
    free(R);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > a7525_have) {
        idx_t D = a7525_have * 2;
        if (D < n + 20) D = n + 20;
        if (D < 64) D = 64;
        a7525_compute(D);
    }
    return (seq_t)a7525_dig[n];
}
