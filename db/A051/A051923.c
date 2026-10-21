/*
 * A051923  Partial sums of A051836.
 * https://oeis.org/A051923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5)*(n+2)/2
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 92, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom5.h"

seq_t a(idx_t n) {
    /* a(n) = binomial(n+5,5) * (n+2) / 2 */
    seq_t bin = binom5(n + 5);
    seq_t result = bin * ((seq_t)n + 2);
    result = result / 2;
    return result;
}
