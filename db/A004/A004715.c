/*
 * A004715  Positions of ones in binary expansion of arctan(1/2)/Pi.
 * https://oeis.org/A004715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of the n-th 1 bit after the binary point of
 *            arctan(1/2)/Pi. Computed in fixed point: arctan(1/m) =
 *            Sum_{k>=0} (-1)^k/((2k+1) m^(2k+1)), Pi = 4*(arctan(1/2) +
 *            arctan(1/3)), then binary long division with guard bits
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a4715_add, a4715_atan, a4715_cmp, a4715_divsmall, a4715_iszero,
 *            a4715_shl, a4715_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a4715_add.h"
#include "../utils/a4715_atan.h"
#include "../utils/a4715_cmp.h"
#include "../utils/a4715_divsmall.h"
#include "../utils/a4715_iszero.h"
#include "../utils/a4715_shl.h"
#include "../utils/a4715_sub.h"

static uint32_t *a4715_pos = NULL;
static idx_t a4715_cnt = 0;

static void a4715_compute(idx_t P)
{
    int L = (int)((P + 96) / 32) + 2;
    uint32_t *s2 = (uint32_t *)malloc(sizeof(uint32_t) * L);
    uint32_t *s3 = (uint32_t *)malloc(sizeof(uint32_t) * L);
    uint32_t *t = (uint32_t *)malloc(sizeof(uint32_t) * L);
    uint32_t *u = (uint32_t *)malloc(sizeof(uint32_t) * L);
    idx_t i, cnt = 0;

    a4715_atan(s2, 2, L, t, u);
    a4715_atan(s3, 3, L, t, u);
    /* t = Pi * 2^W */
    memcpy(t, s2, sizeof(uint32_t) * L);
    a4715_add(t, s3, L);
    a4715_shl(t, L, 2);

    free(a4715_pos);
    a4715_pos = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)(P + 1));
    /* r = arctan(1/2) * 2^W, long division by Pi */
    memcpy(u, s2, sizeof(uint32_t) * L);
    for (i = 1; i <= P; i++) {
        a4715_shl(u, L, 1);
        if (a4715_cmp(u, t, L) >= 0) {
            a4715_sub(u, t, L);
            a4715_pos[cnt++] = (uint32_t)i;
        }
    }
    a4715_cnt = cnt;
    free(s2); free(s3); free(t); free(u);
}

seq_t a(idx_t n)
{
    if (n > a4715_cnt) {
        idx_t P = 2 * n + 128;
        for (;;) {
            a4715_compute(P);
            if (a4715_cnt >= n) break;
            P *= 2;
        }
    }
    return (seq_t)a4715_pos[n - 1];
}
