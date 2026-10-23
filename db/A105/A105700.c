/*
 * A105700  a(n)=1 if n is a prime, 2 if n is a semiprime, otherwise 0.
 * https://oeis.org/A105700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n is prime, 2 if n is a semiprime, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_semiprime_uint64.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    if (is_prime(n)) return (seq_t)1;
    if (is_semiprime_uint64(n)) return (seq_t)2;
    return (seq_t)0;
}
