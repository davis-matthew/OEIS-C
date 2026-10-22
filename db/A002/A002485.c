/*
 * A002485  Numerators of convergents to Pi.
 * https://oeis.org/A002485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   a(n) = c(n-2)*a(n-1) + a(n-2), a(0) = 0, a(1) = 1, where c(k)
 *            are the partial quotients of the continued fraction of Pi
 *            (A001203), computed exactly from rigorous lower/upper bounds on
 *            Pi (Machin's formula) via Euclid's algorithm
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 19, u64 n <= 34, u128 n <= 78,
 *            big any n
 * Helpers:   pc_add, pc_atan_inv, pc_bitlen, pc_cmp, pc_divs, pc_est,
 *            pc_euclid, pc_iszero, pc_muls, pc_sub, pc_submul, pc_top
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pc_add.h"
#include "../utils/pc_atan_inv.h"
#include "../utils/pc_bitlen.h"
#include "../utils/pc_cmp.h"
#include "../utils/pc_divs.h"
#include "../utils/pc_est.h"
#include "../utils/pc_euclid.h"
#include "../utils/pc_iszero.h"
#include "../utils/pc_muls.h"
#include "../utils/pc_sub.h"
#include "../utils/pc_submul.h"
#include "../utils/pc_top.h"

static uint64_t *pc_q = NULL;
static size_t pc_qlen = 0;

static void pc_compute(size_t T) {
    size_t FL = (T * 4 + 256) / 32 + 2;
    for (;;) {
        size_t L = FL + 2, common = 0, c1, c2;
        uint32_t *A = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *B = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *w = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *d1 = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *d2 = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint64_t *q1 = (uint64_t *)malloc((T + 2) * sizeof(uint64_t));
        uint64_t *q2 = (uint64_t *)malloc((T + 2) * sizeof(uint64_t));
        uint32_t one[2];

        pc_atan_inv(A, L, FL, 5, t, w);
        pc_atan_inv(B, L, FL, 239, t, w);
        pc_muls(A, L, 16);
        pc_muls(B, L, 4);
        pc_sub(A, B, L, L);           /* A ~ Pi * 2^(32*FL) */
        memcpy(B, A, L * sizeof(uint32_t));
        one[0] = 0; one[1] = 1;       /* error margin 2^32 ulps */
        pc_sub(A, one, 2, L);         /* lower bound */
        pc_add(B, one, 2, L);         /* upper bound */
        d1[FL] = 1;
        d2[FL] = 1;

        c1 = pc_euclid(A, d1, L, q1, T + 2);
        c2 = pc_euclid(B, d2, L, q2, T + 2);
        while (common < c1 && common < c2 && q1[common] == q2[common]) common++;
        if (common > 0) common--;

        free(A); free(B); free(t); free(w); free(d1); free(d2); free(q2);
        if (common >= T) {
            free(pc_q);
            pc_q = q1;
            pc_qlen = common;
            return;
        }
        free(q1);
        FL *= 2;
    }
}

seq_t a(idx_t n) {
    seq_t p2, p1;
    idx_t i;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if ((size_t)(n - 1) > pc_qlen) {
        size_t T = (size_t)(n - 1);
        if (T < 2 * pc_qlen) T = 2 * pc_qlen;
        if (T < 1024) T = 1024;
        pc_compute(T);
    }
    p2 = 1;
    p1 = (seq_t)pc_q[0];
    for (i = 1; i + 1 < n; i++) {
        seq_t p = (seq_t)pc_q[i] * p1 + p2;
        p2 = p1;
        p1 = p;
    }
    return p1;
}
