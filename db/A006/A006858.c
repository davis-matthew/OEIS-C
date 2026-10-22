/*
 * A006858  Expansion of g.f. x*(1 + x)*(1 + 6*x + x^2)/(1 - x)^7.
 * https://oeis.org/A006858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(2*n+4, 5)/12
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 71, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t coeff = ((seq_t)n + 1) * binom(2 * n + 4, 5);
    return coeff / 12;
}
