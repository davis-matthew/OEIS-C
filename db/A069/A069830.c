/*
 * A069830  Multiplicative inverse of prime(n) modulo prime(n+1).
 * https://oeis.org/A069830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = modular inverse of prime(n) modulo prime(n+1)
 * Range:     u8 n <= 53, u16 n <= 6800, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Get the n-th and (n+1)-th primes. */
    uint32_t p = get_prime(n);     /* prime(n)   */
    uint32_t q = get_prime(n + 1); /* prime(n+1) */

    /* The inverse of p modulo q is p^(q-2) mod q, since q is prime. */
    seq_t inv = mod_pow((seq_t)p, (uint64_t)q - 2, (seq_t)q);

    return inv;
}
