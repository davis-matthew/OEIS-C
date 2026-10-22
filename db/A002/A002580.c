/*
 * A002580  Decimal expansion of cube root of 2.
 * https://oeis.org/A002580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = floor(2^(1/3) * 10^(n-1)) mod 10. 2^(1/3) = 2*y^2 with y
 *            = 2^(-1/3), computed by the division-free Newton iteration y <-
 *            y + y*(1 - 2*y^3)/3 in multi-limb fixed-point arithmetic; the
 *            decimal digits are read off by repeatedly multiplying by 10
 * Range:     u8 n <= 9714, u16 n <= 9714, u32 n <= 9714, u64 n <= 9714,
 *            u128 n <= 9714, big any n
 * Helpers:   a002580_add, a002580_cmp, a002580_div3, a002580_iszero,
 *            a002580_mulfx, a002580_shl1, a002580_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a002580_add.h"
#include "../utils/a002580_cmp.h"
#include "../utils/a002580_div3.h"
#include "../utils/a002580_iszero.h"
#include "../utils/a002580_mulfx.h"
#include "../utils/a002580_shl1.h"
#include "../utils/a002580_sub.h"

static unsigned char *a002580_dig = NULL;
static idx_t a002580_cap = 0;

static void a002580_build(idx_t cap) {
    size_t W = (size_t)((cap * 3322) / 1000 + 64) / 32 + 3;
    size_t L = W + 1;
    uint32_t *y = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *one = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
    int iters = 3, it;
    idx_t k;
    size_t i;

    one[W] = 1;
    y[W - 1] = (uint32_t)(0.7937005259840998 * 4294967296.0);
    while (((size_t)1 << iters) < W + 2) iters++;
    iters += 3;

    for (it = 0; it < iters; it++) {
        int neg;
        a002580_mulfx(t, y, y, W, tmp);
        a002580_mulfx(t2, t, y, W, tmp);
        a002580_shl1(t2, L);
        if (a002580_cmp(t2, one, L) <= 0) { a002580_sub(d, one, t2, L); neg = 0; }
        else { a002580_sub(d, t2, one, L); neg = 1; }
        if (a002580_iszero(d, L)) break;
        a002580_mulfx(t, y, d, W, tmp);
        a002580_div3(t, L);
        if (neg) a002580_sub(y, y, t, L);
        else a002580_add(y, t, L);
    }

    /* c = 2*y^2 */
    a002580_mulfx(t, y, y, W, tmp);
    a002580_shl1(t, L);

    free(a002580_dig);
    a002580_dig = (unsigned char *)malloc((size_t)cap);
    a002580_dig[0] = (unsigned char)t[W];
    for (k = 1; k < cap; k++) {
        uint64_t carry = 0;
        for (i = 0; i < W; i++) {
            uint64_t cur = (uint64_t)t[i] * 10 + carry;
            t[i] = (uint32_t)cur;
            carry = cur >> 32;
        }
        a002580_dig[k] = (unsigned char)carry;
    }
    a002580_cap = cap;

    free(y); free(t); free(t2); free(d); free(one); free(tmp);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > a002580_cap) {
        idx_t cap = a002580_cap * 2;
        if (cap < n) cap = n;
        if (cap < 128) cap = 128;
        a002580_build(cap);
    }
    return (seq_t)a002580_dig[n - 1];
}
