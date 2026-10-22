/*
 * A000645  Number of alkyls X^{II} C_n H_{2n+1} Y with n carbon atoms.
 * https://oeis.org/A000645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: A(x) = B(x)/(1 - C(x)), where B(x) = A000598(x) satisfies
 *            B(x) = 1 + x*(B(x)^3 + 3*B(x)*B(x^2) + 2*B(x^3))/6 and C(x) =
 *            A000642(x) = x*(B(x)^2 + B(x^2))/2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *Bv = NULL, *S2 = NULL, *S3 = NULL, *Cv = NULL, *Dv = NULL, *Av = NULL;
static idx_t done = 0, cap = 0;

static seq_t *grow_one(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = (seq_t *)calloc((size_t)newlen, sizeof(seq_t));
    idx_t i;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) free(old);
    return p;
}

static void ensure(idx_t len) {
    idx_t nc;
    if (len <= cap) return;
    nc = cap * 2;
    if (nc < len) nc = len;
    if (nc < 64) nc = 64;
    Bv = grow_one(Bv, done, nc);
    S2 = grow_one(S2, done, nc);
    S3 = grow_one(S3, done, nc);
    Cv = grow_one(Cv, done, nc);
    Dv = grow_one(Dv, done, nc);
    Av = grow_one(Av, done, nc);
    cap = nc;
}

static void step(idx_t m) {
    idx_t i;
    seq_t t;
    /* b(m) */
    if (m == 0) {
        Bv[0] = 1;
    } else {
        idx_t r = m - 1;
        seq_t p = 0;
        t = S3[r];
        for (i = 0; 2 * i <= r; i++) p += Bv[i] * Bv[r - 2 * i];
        t += p * 3;
        if (r % 3 == 0) t += Bv[r / 3] * 2;
        Bv[m] = t / 6;
    }
    /* B^2 and B^3 coefficients */
    t = 0;
    for (i = 0; i <= m; i++) t += Bv[i] * Bv[m - i];
    S2[m] = t;
    t = 0;
    for (i = 0; i <= m; i++) t += S2[i] * Bv[m - i];
    S3[m] = t;
    /* c(m) = A000642 */
    if (m == 0) {
        Cv[0] = 0;
    } else {
        idx_t r = m - 1;
        t = S2[r];
        if (r % 2 == 0) t += Bv[r / 2];
        Cv[m] = t / 2;
    }
    /* D = 1/(1-C) */
    if (m == 0) {
        Dv[0] = 1;
    } else {
        t = 0;
        for (i = 1; i <= m; i++) t += Cv[i] * Dv[m - i];
        Dv[m] = t;
    }
    /* A = B*D */
    t = 0;
    for (i = 0; i <= m; i++) t += Bv[i] * Dv[m - i];
    Av[m] = t;
}

seq_t a(idx_t n) {
    if (n >= done) {
        ensure(n + 1);
        while (done <= n) {
            step(done);
            done++;
        }
    }
    return Av[n];
}
