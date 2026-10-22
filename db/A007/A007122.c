/*
 * A007122  Number of unlabeled identity connected unit interval graphs with n
 *          nodes.
 * https://oeis.org/A007122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (A005043(n-1) - S(n))/2, where S(2k+1) = A002426(k),
 *            S(2k) = (A002426(k) - A002426(k-1))/2; A005043(m) =
 *            (m-1)*(2*A005043(m-1) + 3*A005043(m-2))/(m+1), A005043(0) = 1,
 *            A005043(1) = 0; A002426(k) = ((2k-1)*A002426(k-1) +
 *            3*(k-1)*A002426(k-2))/k, A002426(0) = A002426(1) = 1
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 27, u64 n <= 48, u128 n <= 89,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

typedef struct { uint64_t w[3]; } l3;

static void l3_add(l3 *d, const l3 *x) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < 3; i++) {
        uint64_t s = d->w[i] + x->w[i];
        uint64_t c1 = s < x->w[i];
        uint64_t s2 = s + c;
        uint64_t c2 = s2 < c;
        d->w[i] = s2;
        c = c1 | c2;
    }
}

static void l3_sub(l3 *d, const l3 *x) {
    uint64_t b = 0;
    int i;
    for (i = 0; i < 3; i++) {
        uint64_t s = d->w[i] - x->w[i];
        uint64_t b1 = d->w[i] < x->w[i];
        uint64_t s2 = s - b;
        uint64_t b2 = s < b;
        d->w[i] = s2;
        b = b1 | b2;
    }
}

static void l3_shr1(l3 *d) {
    d->w[0] = (d->w[0] >> 1) | (d->w[1] << 63);
    d->w[1] = (d->w[1] >> 1) | (d->w[2] << 63);
    d->w[2] >>= 1;
}

static seq_t l3_to_seq(const l3 *x) {
    seq_t r = 0;
    int i, b;
    for (i = 2; i >= 0; i--)
        for (b = 63; b >= 0; b--) {
            r += r;
            if ((x->w[i] >> b) & 1) r += 1;
        }
    return r;
}

/* Riordan number r(m) via Motzkin paths with no level steps at height 0 */
static l3 riordan_l3(idx_t m) {
    l3 *p = (l3 *)calloc((size_t)(m + 3), sizeof(l3));
    l3 *q = (l3 *)calloc((size_t)(m + 3), sizeof(l3));
    l3 res;
    idx_t s, h;
    p[0].w[0] = 1;
    for (s = 1; s <= m; s++) {
        idx_t hmax = s < m - s ? s : m - s;
        for (h = 0; h <= hmax; h++) {
            l3 v = p[h + 1];
            if (h > 0) { l3_add(&v, &p[h - 1]); l3_add(&v, &p[h]); }
            q[h] = v;
        }
        { l3 *t = p; p = q; q = t; }
    }
    res = p[0];
    free(p);
    free(q);
    return res;
}

/* central trinomial coefficients t(j) and t(j-1) */
static void trinom_l3(idx_t j, l3 *tj, l3 *tjm1) {
    l3 *p = (l3 *)calloc((size_t)(j + 2), sizeof(l3));
    idx_t s, i;
    p[0].w[0] = 1;
    *tjm1 = p[0];
    for (s = 1; s <= j; s++) {
        idx_t top = 2 * s < j ? 2 * s : j;
        if (s == j && j >= 1) *tjm1 = p[j - 1];
        for (i = top; i >= 1; i--) {
            l3_add(&p[i], &p[i - 1]);
            if (i >= 2) l3_add(&p[i], &p[i - 2]);
        }
    }
    *tj = p[j];
    free(p);
}

seq_t a(idx_t n) {
    idx_t m = n - 1;
    idx_t j = (n % 2 == 1) ? (n - 1) / 2 : n / 2;
    if (!seq_is_fixed_width()) {
        seq_t r, r0 = 1, r1 = 0, tp = 1, tc = 1, S;
        idx_t k;
        if (m == 0) r = 1;
        else if (m == 1) r = 0;
        else {
            for (k = 2; k <= m; k++) {
                seq_t rk = (seq_t)(k - 1) * (r1 * 2 + r0 * 3) / (seq_t)(k + 1);
                r0 = r1;
                r1 = rk;
            }
            r = r1;
        }
        for (k = 2; k <= j; k++) {
            seq_t tn = ((seq_t)(2 * k - 1) * tc + (seq_t)(3 * (k - 1)) * tp) / (seq_t)k;
            tp = tc;
            tc = tn;
        }
        S = (n % 2 == 1) ? tc : (tc - tp) / 2;
        return (r - S) / 2;
    } else {
        l3 r = riordan_l3(m);
        l3 tj, tjm1, S;
        trinom_l3(j, &tj, &tjm1);
        if (n % 2 == 1) S = tj;
        else {
            S = tj;
            l3_sub(&S, &tjm1);
            l3_shr1(&S);
        }
        l3_sub(&r, &S);
        l3_shr1(&r);
        return l3_to_seq(&r);
    }
}
