/*
 * A000641  Number of paraffins C_n H_{2n-1} X_3 with n carbon atoms.
 * https://oeis.org/A000641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. A(x) = ((x*r)/6)*( 1/(1-x*R)^3 + 3/((1-x*R)*(1-x^2*R2)) +
 *            2/(1-x^3*R3) ), r = A000598(x) = 1 + x*(r^3 + 3*r*r(x^2) +
 *            2*r(x^3))/6, R = A000642(x)/x = (r^2 + r(x^2))/2, Ri = R(x^i).
 *            With f = 1/(1-x*R): a(n) = [x^(n-1)] r * Z(S3; f), where Z(S3;
 *            f) = (f^3 + 3 f f(x^2) + 2 f(x^3))/6. Computed as...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   grow_one, mset2, tri_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_one.h"
#include "../utils/mset2.h"
#include "../utils/tri_m.h"

static int fixedw = -1;
static seq_t inv3;

static seq_t *ra = NULL, *Ra = NULL, *fa = NULL, *Ga = NULL, *Za = NULL, *Aa = NULL;
static idx_t cap = 0;   /* arrays have cap+2 entries */
static idx_t done = 0;  /* iterations k = 0..done-1 completed */

static void init_width(void) {
    seq_t x = 1;
    int i;
    if (fixedw >= 0) return;
    for (i = 0; i < 300; i++) x += x;
    fixedw = (x == 0) ? 1 : 0;
    if (fixedw) {
        seq_t y = 3;
        for (i = 0; i < 8; i++) y *= (seq_t)2 - (seq_t)3 * y;
        inv3 = y;
    }
}

/* exact v/3, where v may have wrapped in fixed width but v/3 fits */
static seq_t d3(seq_t v) {
    if (fixedw) return v * inv3;
    return v / 3;
}

static void grow_all(idx_t newcap) {
    idx_t oldlen = cap ? cap + 2 : 0;
    idx_t newlen = newcap + 2;
    ra = grow_one(ra, oldlen, newlen);
    Ra = grow_one(Ra, oldlen, newlen);
    fa = grow_one(fa, oldlen, newlen);
    Ga = grow_one(Ga, oldlen, newlen);
    Za = grow_one(Za, oldlen, newlen);
    Aa = grow_one(Aa, oldlen, newlen);
    cap = newcap;
}

/* multisets of size 3 of g at weight k, given h2 = mset2(g) */
static seq_t mset3(const seq_t *g, const seq_t *h2, idx_t k) {
    seq_t s = 0;
    idx_t i;
    for (i = 0; i <= k; i++) s += g[i] * h2[k - i];
    for (i = 0; 2 * i <= k; i++) s += g[i] * g[k - 2 * i];
    if (k % 3 == 0) s += g[k / 3];
    return d3(s);
}

static void step(idx_t k) {
    idx_t i;
    seq_t s;
    /* R_k = [x^k] (r^2 + r(x^2))/2 */
    Ra[k] = mset2(ra, k);
    /* r_{k+1} = [x^k] Z(S3; r) */
    ra[k + 1] = mset3(ra, Ra, k);
    /* f = 1/(1 - x R) */
    if (k == 0) {
        fa[0] = 1;
    } else {
        s = 0;
        for (i = 1; i <= k; i++) s += Ra[i - 1] * fa[k - i];
        fa[k] = s;
    }
    Ga[k] = mset2(fa, k);
    Za[k] = mset3(fa, Ga, k);
    /* a(k+1) = [x^k] r * Z */
    s = 0;
    for (i = 0; i <= k; i++) s += ra[i] * Za[k - i];
    Aa[k + 1] = s;
}

seq_t a(idx_t n) {
    idx_t k;
    if (n == 0) return 0;
    init_width();
    if (done < n) {
        if (cap < n) {
            idx_t nc = cap + cap / 2 + 8;
            if (nc < n) nc = n;
            grow_all(nc);
            if (done == 0) {
                ra[0] = 1;
                Aa[0] = 0;
            }
        }
        for (k = done; k < n; k++) step(k);
        done = n;
    }
    return Aa[n];
}
