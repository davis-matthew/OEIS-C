/*
 * A005732  a(n) = binomial(n+3,6) + binomial(n+1,5) + binomial(n,5).
 * https://oeis.org/A005732
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(n+3,6) + binomial(n+1,5) + binomial(n,5)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 93, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    return binom(n + 3, 6) + binom(n + 1, 5) + binom(n, 5);
}
