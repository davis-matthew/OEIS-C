/*
 * A050997  Fifth powers of primes.
 * https://oeis.org/A050997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n)^5, where p_n is the n-th prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 914, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n < 1u) return (seq_t)0;      /* undefined for n = 0 */
    seq_t prime = get_prime(n);
    return pow_int(prime, (idx_t)5);
}
