/*
 * A004717  Positions of 1's in the binary expansion of log(4)/log(3)-1.
 * https://oeis.org/A004717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 1-bit in the binary expansion of x =
 *            log(4)/log(3) - 1 = log(4/3)/log(3) =
 *            atanh(1/7)/(atanh(1/3)+atanh(1/5)). x is computed exactly to P
 *            bits with big fixed-point integers (atanh series plus long
 *            division), then the bits are scanned
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a004717_add, a004717_atanh, a004717_divsmall, a004717_geq,
 *            a004717_iszero, a004717_shl1, a004717_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a004717_add.h"
#include "../utils/a004717_atanh.h"
#include "../utils/a004717_divsmall.h"
#include "../utils/a004717_geq.h"
#include "../utils/a004717_iszero.h"
#include "../utils/a004717_shl1.h"
#include "../utils/a004717_sub.h"

static long long *a004717_pos = NULL;
static long long a004717_cnt = 0;
static long long a004717_P = 0;

static void a004717_compute(long long P) {
    long long W = P + 96;
    int L = (int)(W / 32) + 3;
    uint32_t *A = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    long long i, cap;
    a004717_atanh(A, L, W, 7);
    a004717_atanh(B, L, W, 3);
    a004717_atanh(B, L, W, 5);
    free(a004717_pos);
    cap = P + 1;
    a004717_pos = (long long *)malloc((size_t)cap * sizeof(long long));
    a004717_cnt = 0;
    for (i = 1; i <= P; i++) {
        a004717_shl1(A, L);
        if (a004717_geq(A, B, L)) {
            a004717_sub(A, B, L);
            a004717_pos[a004717_cnt++] = i;
        }
    }
    a004717_P = P;
    free(A);
    free(B);
}

seq_t a(idx_t n) {
    if ((long long)n > a004717_cnt) {
        long long P = a004717_P ? a004717_P : 0;
        if (P < 2 * (long long)n + 256) P = 2 * (long long)n + 256;
        for (;;) {
            a004717_compute(P);
            if (a004717_cnt >= (long long)n) break;
            P *= 2;
        }
    }
    return (seq_t)a004717_pos[n - 1];
}
