/*
 * A076350  A076341(A000290(n)), imaginary part of squares mapped as defined
 *          in A076340, A076341.
 * https://oeis.org/A076350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Im f(n^2), f multiplicative with f(2)=(2,0), f(p)=(p-1,1)
 *            for p==1 (mod 4), f(p)=(p+1,-1) for p==3 (mod 4)
 *            (A076341(A000290(n)))
 * Range:     i8 n <= 8, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void A076350_mul(seq_t *r, seq_t *i, seq_t br, seq_t bi)
{
    seq_t nr = (*r) * br - (*i) * bi;
    seq_t ni = (*r) * bi + (*i) * br;
    *r = nr;
    *i = ni;
}

static void A076350_apply(seq_t *r, seq_t *i, idx_t p, idx_t e)
{
    seq_t br, bi;
    if (p == 2) { br = 2; bi = 0; }
    else if ((p & 3) == 1) { br = (seq_t)p - 1; bi = 1; }
    else { br = (seq_t)p + 1; bi = -1; }
    idx_t k = 2 * e;
    seq_t rr = 1, ri = 0;
    while (k > 0) {
        if (k & 1) A076350_mul(&rr, &ri, br, bi);
        seq_t sr = br, si = bi;
        br = sr * sr - si * si;
        bi = 2 * sr * si;
        k >>= 1;
    }
    A076350_mul(r, i, rr, ri);
}

seq_t a(idx_t n)
{
    seq_t r = 1, im = 0;
    idx_t m = n;
    idx_t e = 0;
    while (m > 1 && (m & 1) == 0) { m >>= 1; e++; }
    if (e) A076350_apply(&r, &im, 2, e);
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) { m /= p; e++; }
            A076350_apply(&r, &im, p, e);
        }
    }
    if (m > 1) A076350_apply(&r, &im, m, 1);
    return im;
}
