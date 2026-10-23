/*
 * A027172  a(n) = (1/2) * A027170(2n, n).
 * https://oeis.org/A027172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+2, n+1) + binomial(2*n-1, n-1) - 2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Computation according to the OEIS formula */
    seq_t term1 = binomial(2*n + 2, n + 1);
    seq_t term2 = binomial(2*n - 1, n - 1);
    const seq_t two = (seq_t)2;
    return term1 + term2 - two;
}
