/*
 * A001919  Eighth column of quadrinomial coefficients.
 * https://oeis.org/A001919
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*C(n,3) + 16*C(n,4) + 15*C(n,5) + 6*C(n,6) + C(n,7)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a001919_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001919_binom.h"

seq_t a(idx_t n) {
    seq_t result = (seq_t)6 * a001919_binom(n, 3)
                 + (seq_t)16 * a001919_binom(n, 4)
                 + (seq_t)15 * a001919_binom(n, 5)
                 + (seq_t)6 * a001919_binom(n, 6)
                 + a001919_binom(n, 7);
    return result;
}
