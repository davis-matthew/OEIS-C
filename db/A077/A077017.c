/*
 * A077017  a(1) = 2, a(n+1) = smallest positive integer divisible by the n-th
 *          prime that also has a nontrivial common divisor with a(n).
 * https://oeis.org/A077017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*prime(n-1) for n>2; a(1) = 2, a(2) = 2
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)2;
    return (seq_t)2 * (seq_t)get_prime(n - 1);
}
