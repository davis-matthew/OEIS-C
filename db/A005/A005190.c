/*
 * A005190  Central quadrinomial coefficients: largest coefficient of (1 + x +
 *          x^2 + x^3)^n.
 * https://oeis.org/A005190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j} binomial(n,j)*binomial(n,k-2j), where k =
 *            floor(3n/2), since (1+x+x^2+x^3)^n = (1+x)^n (1+x^2)^n and the
 *            maximal coefficient is that of x^k
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   bin_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bin_mul.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t k = (3 * n) / 2;
    idx_t h = n / 2;            /* k - n */
    idx_t jmin = (h + 1) / 2;   /* ceil((k-n)/2) */
    idx_t jmax = k / 2;
    if (jmax > n) jmax = n;

    seq_t cj = bin_mul(n, jmin);
    idx_t m = k - 2 * jmin;
    seq_t cm = bin_mul(n, m);
    seq_t sum = 0;

    for (idx_t j = jmin;; j++) {
        sum += cj * cm;
        if (j >= jmax) break;
        /* C(n,j) -> C(n,j+1) */
        cj = cj * (seq_t)(n - j);
        cj = cj / (seq_t)(j + 1);
        /* C(n,m) -> C(n,m-1) -> C(n,m-2) */
        cm = cm * (seq_t)m;
        cm = cm / (seq_t)(n - m + 1);
        cm = cm * (seq_t)(m - 1);
        cm = cm / (seq_t)(n - m + 2);
        m -= 2;
    }
    return sum;
}
