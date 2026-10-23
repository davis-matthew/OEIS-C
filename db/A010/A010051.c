/*
 * A010051  Characteristic function of primes: 1 if n is prime, else 0.
 * https://oeis.org/A010051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is prime, else 0 (characteristic function of
 *            primes)
 * Range:     u8 n <= 8666, u16 n <= 8666, u32 n <= 8666, u64 n <= 8666,
 *            u128 n <= 8666, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    return (seq_t)is_prime(n);
}
