/*
 * A167480  a(n)= primepi(n) if n is prime, otherwise a(n)=prime(n).
 * https://oeis.org/A167480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime_pi(n) if n is prime, otherwise a(n) = the n-th
 *            prime
 * Range:     u8 n <= 54, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   get_prime, is_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x) */
#include "../utils/prime_pi.h"   /* seq_t prime_pi(idx_t limit) */
#include "../utils/get_prime.h"  /* uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for offset < 1 */
    if (is_prime(n))                         /* n is prime */
        return prime_pi(n);                  /* π(n) */
    else
        return (seq_t) get_prime(n);         /* n-th prime */
}
