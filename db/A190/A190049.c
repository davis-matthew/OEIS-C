/*
 * A190049  Expansion of g.f. 2*(8+12*x+x^2)/(x-1)^6.
 * https://oeis.org/A190049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*binomial(n+5,5) + 24*binomial(n+4,5) +
 *            2*binomial(n+3,5)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 101, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t t1 = 16 * binomial(n + 5, 5);
    seq_t t2 = 24 * binomial(n + 4, 5);
    seq_t t3 = 2 * binomial(n + 3, 5);
    return t1 + t2 + t3;
}
