/*
 * A000502  Number of genus 0 rooted maps with 6 faces and n vertices.
 * https://oeis.org/A000502
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.:
 *            x^4*(1-sqrt(1-4*x))*(105+92*x-(84+76*x)*sqrt(1-4*x))/(1-4*x)^7 =
 *            x^4*((189-168*x-304*x^2)/(1-4*x)^7 -
 *            (189+168*x)/(1-4*x)^(13/2)). With m = n-4, T(k) =
 *            binomial(k+6,6)*4^k and B(k) = [x^k](1-4x)^(-13/2) =
 *            2^k*Product_{j=0..k-1}(2j+13)/k!: a(n) = 189*T(m) - 168*T(m-1) -
 *            304*T(m-2) - 189*B(m)...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 52,
 *            big any n
 * Helpers:   b502_inv, b502_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/b502_inv.h"
#include "../utils/b502_mul.h"

static int b502_mode = -1; /* 1 = arbitrary precision, 0 = fixed width */

static int b502_big(void) {
    if (b502_mode < 0) {
        seq_t x = 1;
        int i;
        for (i = 0; i < 200; i++) x = (seq_t)(x + x);
        b502_mode = (x != 0) ? 1 : 0;
    }
    return b502_mode;
}

/* binomial(k+6,6)*4^k */
static seq_t b502_T(idx_t k) {
    seq_t c, p = 1;
    idx_t i;
    if (b502_big()) {
        c = 1;
        for (i = 1; i <= 6; i++) c = c * (seq_t)(k + i) / (seq_t)i;
    } else {
        unsigned long long cc = 1;
        for (i = 1; i <= 6; i++) cc = cc * (unsigned long long)(k + i) / (unsigned long long)i;
        c = (seq_t)cc;
    }
    for (i = 0; i < k; i++) p = b502_mul(p, (seq_t)4);
    return b502_mul(c, p);
}

/* [x^k] (1-4x)^(-13/2) */
static seq_t b502_B(idx_t k) {
    idx_t j;
    if (b502_big()) {
        seq_t b = 1;
        for (j = 1; j <= k; j++) {
            b = b * (seq_t)(4 * j + 22);
            b = b / (seq_t)j;
        }
        return b;
    } else {
        seq_t P = 1, O = 1, r;
        idx_t v = 0, e, i;
        for (j = 1; j <= k; j++) {
            idx_t jj = j;
            P = b502_mul(P, (seq_t)(2 * j + 11));
            while ((jj & 1) == 0) { jj >>= 1; v++; }
            O = b502_mul(O, (seq_t)jj);
        }
        e = k - v;
        r = b502_mul(P, b502_inv(O));
        for (i = 0; i < e && i < 200; i++) r = (seq_t)(r + r);
        return r;
    }
}

seq_t a(idx_t n) {
    idx_t m;
    seq_t pos, neg;
    if (n < 5) return 0;
    m = n - 4;
    pos = b502_mul((seq_t)189, b502_T(m));
    neg = b502_mul((seq_t)189, b502_B(m));
    if (m >= 1) {
        neg = (seq_t)(neg + b502_mul((seq_t)168, b502_T(m - 1)));
        neg = (seq_t)(neg + b502_mul((seq_t)168, b502_B(m - 1)));
    }
    if (m >= 2) {
        neg = (seq_t)(neg + b502_mul((seq_t)304, b502_T(m - 2)));
    }
    return (seq_t)(pos - neg);
}
