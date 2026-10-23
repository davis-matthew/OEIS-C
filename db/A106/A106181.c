/*
 * A106181  Expansion of c(-x^2)(1+2x-sqrt(1+4x^2))/2, c(x) the g.f. of
 *          A000108.
 * https://oeis.org/A106181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^⌊n/2⌋ * Catalan(⌊n/2⌋) for n ≥ 1, a(0) = 0
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 29, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    idx_t k = n / 2;                     /* floor(n/2) */
    seq_t cat = binom(2 * k, k);         /* binomial(2k, k) */
    cat = cat / (seq_t)(k + 1);          /* divide by (k+1) → Catalan(k) */
    if (k & 1)                           /* apply (−1)^k sign */
        cat = -cat;
    return cat;
}
