/*
 * A007010  Number of 4-voter voting schemes with n linearly ranked choices.
 * https://oeis.org/A007010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x*(1+6*x+21*x^2+28*x^3+21*x^4+6*x^5+x^6)/((1+x)^3*(1-x)^9)
 *            = x*N(x) / ((1-x^2)^3 * (1-x)^6), so with m = n-1: a(n) =
 *            Sum_{k=0..floor(m/2)} binomial(k+2,2) * c(m-2k), c(t) =
 *            Sum_{j=0..6} N_j * binomial(t-j+5,5)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 42, u64 n <= 716, u128 n <= 1000,
 *            big any n
 * Helpers:   a007010_b5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a007010_b5.h"
static const unsigned a007010_N[7] = {1, 6, 21, 28, 21, 6, 1};

static seq_t a007010_c(idx_t t) {
    seq_t s = 0;
    idx_t j;
    for (j = 0; j <= 6 && j <= t; j++) {
        s += (seq_t)a007010_N[j] * a007010_b5(t - j + 5);
    }
    return s;
}

seq_t a(idx_t n) {
    idx_t m = n - 1;
    idx_t k;
    seq_t s = 0;
    for (k = 0; 2 * k <= m; k++) {
        seq_t w = (seq_t)((k + 1) * (k + 2) / 2);
        s += w * a007010_c(m - 2 * k);
    }
    return s;
}
