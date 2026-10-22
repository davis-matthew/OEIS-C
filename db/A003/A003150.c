/*
 * A003150  Fibonomial Catalan numbers.
 * https://oeis.org/A003150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = F(2n)*F(2n-1)*...*F(n+2) / (F(n)*F(n-1)*...*F(1)) =
 *            Product_{d>=3} P(d)^e(d), where P(d) is the primitive part of
 *            F(d) (F(d) = Product_{e|d} P(e)) and e(d) = floor(2n/d) -
 *            floor(n/d) - floor((n+1)/d), which is 0 or 1 for d >= 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t N, d, m;
    seq_t *p;
    seq_t r;

    if (n == 0) return (seq_t)1;
    N = 2 * n;

    /* p[d] starts as F(d) and is sieved down to its primitive part P(d) */
    p = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    p[0] = 0;
    p[1] = 1;
    for (d = 2; d <= N; d++) p[d] = p[d - 1] + p[d - 2];

    for (d = 3; d <= N; d++) {
        /* p[d] is final now; strip it from its proper multiples */
        if (p[d] == 0 || p[d] == 1) continue;
        for (m = 2 * d; m <= N; m += d) p[m] /= p[d];
    }

    r = 1;
    for (d = 3; d <= N; d++) {
        idx_t e = N / d - n / d - (n + 1) / d;
        if (e > 0) r *= p[d];
    }

    free(p);
    return r;
}
