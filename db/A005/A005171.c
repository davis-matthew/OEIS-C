/*
 * A005171  Characteristic function of nonprimes: 0 if n is prime, else 1.
 * https://oeis.org/A005171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - A010051(n) (characteristic function of nonprimes)
 * Range:     u8 n <= 9426, u16 n <= 9426, u32 n <= 9426, u64 n <= 9426,
 *            u128 n <= 9426, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    return is_prime(n) ? 0 : 1;
}
