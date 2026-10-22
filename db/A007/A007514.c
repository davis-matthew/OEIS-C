/*
 * A007514  Pi = Sum_{n >= 0} a(n)/n!.
 * https://oeis.org/A007514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n!*Pi) - n*floor((n-1)!*Pi), a(0) = 3; computed as
 *            the factorial-base digits of Pi: with f = frac(Pi), a(n) =
 *            floor(n*f_{n-1}), f_n = n*f_{n-1} - a(n), using exact
 *            fixed-point Pi from Machin's formula Pi = 16*atan(1/5) -
 *            4*atan(1/239) at about log2(n!) + guard bits
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a007514_add, a007514_atan_inv, a007514_div25, a007514_div57121,
 *            a007514_divsmall, a007514_mulsmall, a007514_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a007514_add.h"
#include "../utils/a007514_atan_inv.h"
#include "../utils/a007514_div25.h"
#include "../utils/a007514_div57121.h"
#include "../utils/a007514_divsmall.h"
#include "../utils/a007514_mulsmall.h"
#include "../utils/a007514_sub.h"

static uint32_t *a007514_dig = 0;
static idx_t a007514_cap = 0;

static void a007514_compute(idx_t N) {
    uint64_t bits = 128;
    idx_t k;
    for (k = 2; k <= N; k++) {
        idx_t v = k;
        while (v) { bits++; v >>= 1; }
    }
    size_t len = (size_t)(1 + bits / 32 + 3);
    uint32_t *s5 = (uint32_t *)malloc(len * sizeof(uint32_t));
    uint32_t *s239 = (uint32_t *)malloc(len * sizeof(uint32_t));
    uint32_t *t = (uint32_t *)malloc(len * sizeof(uint32_t));
    uint32_t *q = (uint32_t *)malloc(len * sizeof(uint32_t));
    a007514_atan_inv(5, s5, t, q, len);
    a007514_atan_inv(239, s239, t, q, len);
    a007514_mulsmall(s5, len, 16);
    a007514_mulsmall(s239, len, 4);
    a007514_sub(s5, s239, 0, len);  /* s5 = Pi */

    uint32_t *dig = (uint32_t *)malloc((size_t)(N + 1) * sizeof(uint32_t));
    dig[0] = s5[0];
    s5[0] = 0;
    for (k = 1; k <= N; k++) {
        a007514_mulsmall(s5, len, (uint32_t)k);
        dig[k] = s5[0];
        s5[0] = 0;
    }
    free(s5); free(s239); free(t); free(q);
    free(a007514_dig);
    a007514_dig = dig;
    a007514_cap = N + 1;
}

seq_t a(idx_t n) {
    if (n >= a007514_cap) {
        idx_t N = n;
        if (N < a007514_cap + a007514_cap / 4) N = a007514_cap + a007514_cap / 4;
        if (N < 32) N = 32;
        a007514_compute(N);
    }
    return (seq_t)a007514_dig[n];
}
