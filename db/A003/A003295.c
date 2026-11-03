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
 * Validated: standard, pass, 48 terms through n = 995 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* multiply (div=0) or divide (div=1) series f (length L) by eta(q^m) without the q^(m/24) factor */
static void eta_apply(seq_t *f, idx_t L, idx_t m, int div) {
    idx_t cap = 8, cnt = 0, k;
    idx_t *ex;
    int *sg;
    for (k = 1; m * k * (3 * k - 1) / 2 < L; k++) cap += 2;
    ex = (idx_t *)malloc(cap * sizeof(idx_t));
    sg = (int *)malloc(cap * sizeof(int));
    for (k = 1;; k++) {
        idx_t e1 = m * k * (3 * k - 1) / 2, e2 = m * k * (3 * k + 1) / 2;
        int s = (k & 1) ? -1 : 1;
        if (e1 >= L) break;
        ex[cnt] = e1; sg[cnt] = s; cnt++;
        if (e2 < L) { ex[cnt] = e2; sg[cnt] = s; cnt++; }
    }
    if (!div) {
        idx_t i;
        for (i = L - 1; i >= 0; i--) {
            seq_t s = f[i];
            idx_t j;
            for (j = 0; j < cnt && ex[j] <= i; j++) {
                if (sg[j] > 0) s += f[i - ex[j]];
                else s -= f[i - ex[j]];
            }
            f[i] = s;
        }
    } else {
        idx_t i;
        for (i = 0; i < L; i++) {
            seq_t s = f[i];
            idx_t j;
            for (j = 0; j < cnt && ex[j] <= i; j++) {
                if (sg[j] > 0) s -= f[i - ex[j]];
                else s += f[i - ex[j]];
            }
            f[i] = s;
        }
    }
    free(ex);
    free(sg);
}

static seq_t *zalloc_vals(idx_t len) {
    /* zero-filled storage: an all-zero object is a valid zero value */
    return mem_new(seq_t, (size_t)len);
}

static seq_t *gpow(idx_t L, int p) {
    seq_t *f = zalloc_vals(L);
    int t;
    f[0] = 1;
    if (p > 0) {
        for (t = 0; t < p; t++) {
            eta_apply(f, L, 3, 0);
            eta_apply(f, L, 33, 0);
            eta_apply(f, L, 1, 1);
            eta_apply(f, L, 11, 1);
        }
    } else {
        for (t = 0; t < -p; t++) {
            eta_apply(f, L, 1, 0);
            eta_apply(f, L, 11, 0);
            eta_apply(f, L, 3, 1);
            eta_apply(f, L, 33, 1);
        }
    }
    return f;
}

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
