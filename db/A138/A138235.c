/*
 * A138235  a(n) = floor(n*(6 + sqrt(6))/5).
 * https://oeis.org/A138235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(6 + sqrt(6))/5)
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt6n2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt6n2.h"

seq_t a(idx_t n) {
    seq_t s = isqrt6n2(n);                          /* floor(n*sqrt(6)) */
    __uint128_t num = (__uint128_t)s + (__uint128_t)6 * n; /* s + 6*n */
    return (seq_t)(num / 5);                      /* floor((s+6n)/5) */
}
