/*
 * A005981  Number of 2 up, 2 down, 2 up, ... permutations of length 2n + 1.
 * https://oeis.org/A005981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   with E(x) = x + Sum_{n>=1} a(n)*x^(2n+1)/(2n+1)! = N(x)/D(x),
 *            where D(x) = f(0,x)^2 - f(1,x)*f(3,x) and N(x) = f(0,x)*f(1,x) -
 *            f(2,x)*f(3,x) + f(3,x). Coefficients (e.g.f. form): D[0] = 1,
 *            D[4m] = (-1)^m * 2^(2m-1) (m >= 1); N[4m+1] = (-4)^m, N[4m+3] =
 *            1, all others 0. So, writing e(k) for the co...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *E = NULL;
static idx_t cap = 0;
static idx_t done = 0;

static void compute_k(idx_t k) {
    seq_t pos = 0, neg = 0;
    if (k % 4 == 1) {
        idx_t mm = (k - 1) / 4;
        seq_t p = 1;
        for (idx_t i = 0; i < mm; i++) p *= 4;
        if (mm % 2 == 1) neg += p; else pos += p;
    } else {
        pos += 1;
    }
    seq_t b = 1;   /* binomial(k, 4m) */
    seq_t p2 = 2;  /* 2^(2m-1) */
    for (idx_t m = 1; 4 * m <= k; m++) {
        for (idx_t j = 4 * m - 4; j < 4 * m; j++)
            b = b * (seq_t)(k - j) / (seq_t)(j + 1);
        seq_t term = b * p2 * E[k - 4 * m];
        if (m % 2 == 1) pos += term; else neg += term;
        p2 *= 4;
    }
    E[k] = pos - neg;
}

seq_t a(idx_t n) {
    idx_t K = 2 * n + 1;
    if (K >= cap) {
        idx_t newcap = 2 * K + 2;
        E = grow_zero(E, cap, newcap);
        cap = newcap;
    }
    while (done <= K) {
        if (done % 2 == 1) compute_k(done);
        done++;
    }
    return E[K];
}
