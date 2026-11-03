/*
 * A002711  Triangulations of the disk G_{n,2}.
 * https://oeis.org/A002711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (psi(n) + 4*[n == 1 (mod 5)]*f((n-1)/5)) / 5, where
 *            psi(n) = 210*(4n+5)!/(n!*(3n+7)!) =
 *            210*binomial(4n+5,n)/((3n+6)*(3n+7)) (Brown's count of rooted
 *            disk triangulations, n internal vertices, 5 boundary vertices)
 *            and f(k) = 2*(4k+1)!/(k!*(3k+2)!) = 2*binomial(4k+1,k)/(3k+2)
 *            (rooted ones wi...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 432 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t a002711_mm(seq_t x, seq_t y) {
    return (seq_t)(x * 1u * y);
}

static seq_t a002711_inv(seq_t x) {
    seq_t y = x;
    int i;
    for (i = 0; i < 7; i++) y = a002711_mm(y, (seq_t)(2u - a002711_mm(x, y)));
    return y;
}

static int a002711_isfixed(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x = (seq_t)(x * 2u);
    return x == 0;
}

static void a002711_mul(seq_t *O, long *e, uint64_t v, int div) {
    seq_t s;
    while (v % 2 == 0) {
        v /= 2;
        if (div) (*e)--; else (*e)++;
    }
    s = (seq_t)v;
    *O = a002711_mm(*O, div ? a002711_inv(s) : s);
}

static seq_t a002711_val(seq_t O, long e) {
    for (; e > 0; e--) O = (seq_t)(O * 2u);
    return O;
}

seq_t a(idx_t n) {
    if (a002711_isfixed()) {
        seq_t O = 1, psi, F = 0, tot;
        long e = 0;
        idx_t i;
        a002711_mul(&O, &e, 210, 0);
        for (i = 0; i < n; i++) {
            a002711_mul(&O, &e, 4 * n + 5 - i, 0);
            a002711_mul(&O, &e, i + 1, 1);
        }
        a002711_mul(&O, &e, 3 * n + 6, 1);
        a002711_mul(&O, &e, 3 * n + 7, 1);
        psi = a002711_val(O, e);
        if (n % 5 == 1) {
            idx_t k = (n - 1) / 5;
            seq_t O2 = 1;
            long e2 = 0;
            a002711_mul(&O2, &e2, 2, 0);
            for (i = 0; i < k; i++) {
                a002711_mul(&O2, &e2, 4 * k + 1 - i, 0);
                a002711_mul(&O2, &e2, i + 1, 1);
            }
            a002711_mul(&O2, &e2, 3 * k + 2, 1);
            F = a002711_val(O2, e2);
        }
        tot = (seq_t)(psi + a002711_mm(F, (seq_t)4));
        return a002711_mm(tot, a002711_inv((seq_t)5));
    } else {
        seq_t r = 1, psi, F = 0;
        idx_t i;
        for (i = 0; i < n; i++) {
            r = r * (seq_t)(4 * n + 5 - i);
            r = r / (seq_t)(i + 1);
        }
        psi = r * 210;
        psi = psi / ((seq_t)(3 * n + 6) * (seq_t)(3 * n + 7));
        if (n % 5 == 1) {
            idx_t k = (n - 1) / 5;
            seq_t s = 1;
            for (i = 0; i < k; i++) {
                s = s * (seq_t)(4 * k + 1 - i);
                s = s / (seq_t)(i + 1);
            }
            F = s * 2;
            F = F / (seq_t)(3 * k + 2);
        }
        return (psi + F * 4) / 5;
    }
}
