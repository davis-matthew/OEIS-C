/*
 * A002390  Decimal expansion of natural logarithm of golden ratio.
 * https://oeis.org/A002390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) * log(phi)) mod 10, where log(phi) =
 *            atanh(1/sqrt(5)) = (1/sqrt(5)) * Sum_{k>=0} 5^(-k)/(2k+1);
 *            computed in binary fixed point with 1/sqrt(5) obtained by Newton
 *            iteration y <- y*(3 - 5*y^2)/2
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   a2390_add, a2390_cmp, a2390_divs, a2390_iszero, a2390_mulfx,
 *            a2390_muls, a2390_shr1, a2390_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2390_add.h"
#include "../utils/a2390_cmp.h"
#include "../utils/a2390_divs.h"
#include "../utils/a2390_iszero.h"
#include "../utils/a2390_mulfx.h"
#include "../utils/a2390_muls.h"
#include "../utils/a2390_shr1.h"
#include "../utils/a2390_sub.h"

static uint32_t *a2390_val = NULL;
static size_t a2390_F = 0;

static void a2390_compute(size_t F) {
    size_t L = F + 1;
    uint32_t *S = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *y = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *y2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *one = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *p = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
    uint64_t k, prec;
    int extra;

    /* S = Sum_{k>=0} 5^(-k)/(2k+1) */
    t[F] = 1;
    for (k = 0; !a2390_iszero(t, L); k++) {
        memcpy(tmp, t, L * sizeof(uint32_t));
        a2390_divs(tmp, L, (uint32_t)(2 * k + 1));
        a2390_add(S, tmp, L);
        a2390_divs(t, L, 5);
    }

    /* y = 1/sqrt(5) by Newton */
    one[F] = 1;
    y[F - 1] = 1920767767u;
    prec = 28;
    extra = 3;
    while (prec < 32 * (uint64_t)F + 64 || extra-- > 0) {
        int neg;
        a2390_mulfx(y, y, y2, p, L, F);
        a2390_muls(y2, L, 5);
        if (a2390_cmp(y2, one, L) <= 0) {
            memcpy(tmp, one, L * sizeof(uint32_t));
            a2390_sub(tmp, y2, L);
            neg = 0;
        } else {
            memcpy(tmp, y2, L * sizeof(uint32_t));
            a2390_sub(tmp, one, L);
            neg = 1;
        }
        a2390_mulfx(y, tmp, y2, p, L, F);
        a2390_shr1(y2, L);
        if (neg) a2390_sub(y, y2, L); else a2390_add(y, y2, L);
        if (prec < 32 * (uint64_t)F + 64) prec *= 2;
    }

    a2390_mulfx(S, y, res, p, L, F);

    free(S); free(t); free(tmp); free(y); free(y2); free(one); free(p);
    if (a2390_val) free(a2390_val);
    a2390_val = res;
    a2390_F = F;
}

seq_t a(idx_t n) {
    size_t need = (size_t)((n + 40) / 8 + 3);
    size_t F, L, i;
    uint32_t *w;
    uint32_t dig = 0;
    if (a2390_val == NULL || a2390_F < need) a2390_compute(need);
    F = a2390_F;
    L = F + 1;
    w = (uint32_t *)malloc(L * sizeof(uint32_t));
    memcpy(w, a2390_val, L * sizeof(uint32_t));
    w[F] = 0;
    for (i = 0; i <= (size_t)n; i++) {
        a2390_muls(w, L, 10);
        dig = w[F];
        w[F] = 0;
    }
    free(w);
    return (seq_t)dig;
}
