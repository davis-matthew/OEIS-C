/*
 * A004406  Expansion of 1 / (Sum_{n=-oo..oo} x^(n^2))^5.
 * https://oeis.org/A004406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   with theta_3(x) = Sum_{k=-oo..oo} x^(k^2) and h = theta_3^(-5),
 *            h*theta_3' = -5*theta_3'*h gives a(0) = 1, a(n) = -(2/n) *
 *            Sum_{j>=1, j^2<=n} (n + 4*j^2) * a(n-j^2)
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 20, i64 n <= 63, i128 n <= 212,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *h = NULL;
static idx_t h_len = 0;
static idx_t h_cap = 0;

static void compute_term(idx_t m) {
    seq_t acc = 0;
    seq_t R = 0;
    seq_t sm = (seq_t)m;
    /* even j terms (sign opposite to result) first, then odd j terms,
       so partial sums move monotonically and stay within range */
    for (int pass = 0; pass < 2; pass++) {
        idx_t j = (pass == 0) ? 2 : 1;
        for (; j * j <= m; j += 2) {
            seq_t hv = h[m - j * j];
            seq_t t = (seq_t)(j * j) * hv;
            seq_t qq = t / sm;
            seq_t rr = t - qq * sm;
            acc -= hv;
            acc -= hv;
            acc -= 8 * qq;
            R += rr;
        }
    }
    acc -= (8 * R) / sm;
    h[m] = acc;
}

seq_t a(idx_t n) {
    if (n >= h_len) {
        if (n >= h_cap) {
            idx_t newcap = h_cap * 2;
            if (newcap < n + 1) newcap = n + 1;
            if (newcap < 16) newcap = 16;
            h = grow_zero(h, h_cap, newcap);
            h_cap = newcap;
        }
        if (h_len == 0) {
            h[0] = 1;
            h_len = 1;
        }
        for (idx_t m = h_len; m <= n; m++) compute_term(m);
        h_len = n + 1;
    }
    return h[n];
}
