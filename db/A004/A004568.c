/*
 * A004568  Expansion of sqrt(6) in base 9.
 * https://oeis.org/A004568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(6)*9^(n-1)) mod 9; the base-9 digits are
 *            produced by the digit-by-digit square-root method in base 9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a4568_cmp, a4568_norm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a4568_cmp.h"
#include "../utils/a4568_norm.h"

static unsigned char *a4568_dig = NULL;
static idx_t a4568_have = 0;

static void a4568_compute(idx_t m) {
    long cap = (long)m + 8;
    unsigned char *r = (unsigned char *)calloc((size_t)cap, 1);
    unsigned char *c = (unsigned char *)calloc((size_t)cap, 1);
    unsigned char *u = (unsigned char *)calloc((size_t)cap, 1);
    unsigned char *t = (unsigned char *)calloc((size_t)cap, 1);
    unsigned char *y = (unsigned char *)calloc((size_t)cap, 1);
    unsigned char *nd = (unsigned char *)malloc((size_t)m);
    long lr = 0, ly = 0, lc, lu, lt = 0, j;
    idx_t i;

    for (i = 0; i < m; i++) {
        int ds, d, borrow;
        unsigned carry;

        /* c = 81*r + next pair of radicand digits */
        c[0] = (unsigned char)(i == 0 ? 6 : 0);
        c[1] = 0;
        for (j = 0; j < lr; j++) c[j + 2] = r[j];
        lc = lr + 2;
        a4568_norm(c, &lc);

        /* u = 9*y (y = 2*root so far) */
        u[0] = 0;
        for (j = 0; j < ly; j++) u[j + 1] = y[j];
        lu = ly + 1;

        if (ly == 0) {
            ds = 8;
        } else if (lc < lu) {
            ds = 0;
        } else {
            unsigned long long ct = 0, ut = 0;
            long lo = lc - 15;
            if (lo < 0) lo = 0;
            for (j = lc - 1; j >= lo; j--) {
                ct = ct * 9 + c[j];
                ut = ut * 9 + (j < lu ? u[j] : 0);
            }
            if (ut == 0) ds = 8;
            else {
                unsigned long long q = (ct + 1) / ut;
                ds = q > 8 ? 8 : (int)q;
            }
        }

        for (d = ds; d > 0; d--) {
            u[0] = (unsigned char)d;
            carry = 0;
            for (j = 0; j < lu; j++) {
                unsigned v = (unsigned)u[j] * (unsigned)d + carry;
                t[j] = (unsigned char)(v % 9);
                carry = v / 9;
            }
            lt = lu;
            while (carry) { t[lt++] = (unsigned char)(carry % 9); carry /= 9; }
            a4568_norm(t, &lt);
            if (a4568_cmp(t, lt, c, lc) <= 0) break;
        }
        if (d == 0) lt = 0;

        /* r = c - t */
        borrow = 0;
        for (j = 0; j < lc; j++) {
            int v = (int)c[j] - borrow - (j < lt ? (int)t[j] : 0);
            if (v < 0) { v += 9; borrow = 1; } else borrow = 0;
            r[j] = (unsigned char)v;
        }
        lr = lc;
        a4568_norm(r, &lr);

        /* y = 9*y + 2*d */
        for (j = ly; j > 0; j--) y[j] = y[j - 1];
        y[0] = 0;
        ly++;
        carry = (unsigned)(2 * d);
        j = 0;
        while (carry) {
            unsigned v;
            if (j >= ly) { y[j] = 0; ly = j + 1; }
            v = (unsigned)y[j] + carry;
            y[j] = (unsigned char)(v % 9);
            carry = v / 9;
            j++;
        }
        a4568_norm(y, &ly);

        nd[i] = (unsigned char)d;
    }

    free(r); free(c); free(u); free(t); free(y);
    free(a4568_dig);
    a4568_dig = nd;
    a4568_have = m;
}

seq_t a(idx_t n) {
    if (n > a4568_have) {
        idx_t m = n;
        if (m < 64) m = 64;
        if (a4568_have && m < a4568_have + a4568_have / 4)
            m = a4568_have + a4568_have / 4;
        a4568_compute(m);
    }
    return (seq_t)a4568_dig[n - 1];
}
