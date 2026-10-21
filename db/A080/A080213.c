/*
 * A080213  a(n) = binomial(n, greatest prime factor of n).
 * https://oeis.org/A080213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, largest_prime_divisor(n))
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 33, u64 n <= 73, u128 n <= 133,
 *            big any n
 * Helpers:   binomial, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/largest_prime_divisor.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n <= 1) return 1;            /* a(1) = 1 by definition */
    uint64_t p = largest_prime_divisor(n); /* greatest prime factor of n */
    return binomial(n, p);           /* binomial(n, p) is computed in seq_t */
}
