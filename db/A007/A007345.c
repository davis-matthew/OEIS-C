/*
 * A007345  Number of Havender tableaux of height 2 with n columns.
 * https://oeis.org/A007345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} binomial(n,i) * C(i) * (2^(i+1) - 1), where
 *            C(i) = A000108(i) is the i-th Catalan number (from G.f. with y =
 *            x^2: (sqrt((1-5y)/(1-y)) - sqrt((1-9y)/(1-y)))/(2y),
 *            sqrt(1-4c*z) with z = y/(1-y))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t c = 1;   /* binomial(n,i) * Catalan(i) */
    seq_t p = 2;   /* 2^(i+1) */
    idx_t i;
    for (i = 0; ; i++) {
        sum += c * (p - 1);
        if (i == n) break;
        /* Catalan(i+1) * binomial(n,i) */
        c = c * (seq_t)(4 * i + 2) / (seq_t)(i + 2);
        /* Catalan(i+1) * binomial(n,i+1) */
        c = c * (seq_t)(n - i) / (seq_t)(i + 1);
        p *= 2;
    }
    return sum;
}
