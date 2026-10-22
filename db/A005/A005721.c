/*
 * A005721  Central quadrinomial coefficients.
 * https://oeis.org/A005721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=ceil(n/2)..floor(3n/2)} binomial(2n,
 *            3n-2j)*binomial(2n, j), i.e. the coefficient of x^(3n) in
 *            ((1+x)*(1+x^2))^(2n) = (1+x+x^2+x^3)^(2n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   a005721_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005721_binom.h"

seq_t a(idx_t n) {
    idx_t N = 2 * n;
    idx_t j = (n + 1) / 2;
    idx_t jmax = (3 * n) / 2;
    idx_t k = 3 * n - 2 * j;
    seq_t cj = a005721_binom(N, j);
    seq_t ck = a005721_binom(N, k);
    seq_t sum = 0;

    for (;;) {
        sum += cj * ck;
        if (j >= jmax) break;
        /* advance j -> j+1 */
        cj = cj * (seq_t)(N - j);
        cj = cj / (seq_t)(j + 1);
        j++;
        /* advance k -> k-2 in two exact steps */
        ck = ck * (seq_t)k;
        ck = ck / (seq_t)(N - k + 1);
        k--;
        ck = ck * (seq_t)k;
        ck = ck / (seq_t)(N - k + 1);
        k--;
    }
    return sum;
}
