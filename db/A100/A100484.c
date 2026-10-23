/*
 * A100484  The primes doubled; even semiprimes.
 * https://oeis.org/A100484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * A000040(n)
 * Range:     u8 n <= 31, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, guard for safety */
    return ((seq_t)2) * ((seq_t)get_prime(n));
}
