/*
 * A079261  Characteristic function of primes of form 4n+3 (1 if n is prime of
 *          form 4n+3, 0 otherwise).
 * https://oeis.org/A079261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is prime and n % 4 == 3, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          // >1 needed for primes
    if (!is_prime(n)) return (seq_t)0;   // not a prime
    return ((n & (seq_t)3) == 3) ? (seq_t)1 : (seq_t)0;
}
