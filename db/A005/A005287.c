/*
 * A005287  Number of permutations of [n] with four inversions.
 * https://oeis.org/A005287
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,4) + binomial(n+1,3) - binomial(n+1,2)
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 565, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    return binom(n + 1, 4) + binom(n + 1, 3) - binom(n + 1, 2);
}
