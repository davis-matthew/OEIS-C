/*
 * A001473  Number of degree-n permutations of order exactly 4.
 * https://oeis.org/A001473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f.: exp(x + x^2/2 + x^4/4) - exp(x + x^2/2), i.e. a(n) =
 *            g(n) - h(n) with g(n) = g(n-1) + (n-1)*g(n-2) +
 *            (n-1)*(n-2)*(n-3)*g(n-4), g(0)=g(1)=1, g(2)=2, g(3)=4, h(n) =
 *            h(n-1) + (n-1)*h(n-2), h(0)=h(1)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n_idx) {
    if (n_idx < 1) return 0;

    idx_t n = n_idx;

    seq_t *d_g = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *d_h = (seq_t *)calloc(n + 1, sizeof(seq_t));

    d_g[0] = 1;
    d_h[0] = 1;

    if (n >= 1) {
        d_g[1] = 1;
        d_h[1] = 1;
    }

    if (n >= 2) {
        d_g[2] = d_g[1] + d_g[0];
        d_h[2] = d_h[1] + d_h[0];
    }

    if (n >= 3) {
        d_g[3] = d_g[2] + (seq_t)2 * d_g[1];
        d_h[3] = d_h[2] + (seq_t)2 * d_h[1];
    }

    for (idx_t i = 4; i <= n; i++) {
        seq_t i_minus_1 = (seq_t)(i - 1);
        seq_t i_minus_2 = (seq_t)(i - 2);
        seq_t i_minus_3 = (seq_t)(i - 3);

        d_g[i] = d_g[i-1] + i_minus_1 * d_g[i-2]
               + i_minus_1 * i_minus_2 * i_minus_3 * d_g[i-4];
        d_h[i] = d_h[i-1] + i_minus_1 * d_h[i-2];
    }

    seq_t result = d_g[n] - d_h[n];

    free(d_g);
    free(d_h);

    return result;
}
