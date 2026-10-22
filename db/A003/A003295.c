/*
 * A003295  McKay-Thompson series of class 11A for the Monster group with a(0)
 *          = -5.
 * https://oeis.org/A003295
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(n) = [q^(n+1)] of 1/G - 4*q + 18*q^2*G + 27*q^3*G^2 +
 *            27*q^4*G^3, with G = prod_{k>=1} (1-q^(3k))(1-q^(33k)) /
 *            ((1-q^k)(1-q^(11k))), i.e. -11 + (1+3F)^2*(1/F + 1 + 3F) with F
 *            = eta(q^3)eta(q^33)/(eta(q)eta(q^11)) (A128663)
 * Range:     i8 n <= 3, i16 n <= 12, i32 n <= 44, i64 n <= 163, i128 n <= 612,
 *            big any n
 * Helpers:   eta_apply, gpow, zalloc_vals
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/eta_apply.h"
#include "../utils/gpow.h"
#include "../utils/zalloc_vals.h"

static seq_t *cache = NULL;
static idx_t clen = 0;

static void build(idx_t L) {
    seq_t *gi = gpow(L, -1), *g1 = gpow(L, 1), *g2 = gpow(L, 2), *g3 = gpow(L, 3);
    seq_t *r = zalloc_vals(L);
    idx_t i;
    for (i = 0; i < L; i++) {
        seq_t v = gi[i];
        if (i == 1) v -= 4;
        if (i >= 2) v += 18 * g1[i - 2];
        if (i >= 3) v += 27 * g2[i - 3];
        if (i >= 4) v += 27 * g3[i - 4];
        r[i] = v;
    }
    /* temporaries are intentionally not freed: their elements may own heap
       storage in the arbitrary-precision build, and freeing the raw block
       without destroying them is unsafe to reuse */
    cache = r;
    clen = L;
}

seq_t a(idx_t n) {
    idx_t idx;
    if (n < -1) return 0;
    idx = n + 1;
    if (idx >= clen) {
        idx_t L = idx + 1;
        if (L < 2 * clen) L = 2 * clen;
        if (L < 1024) L = 1024;
        build(L);
    }
    return cache[idx];
}
