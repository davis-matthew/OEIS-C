/*
 * A085064  a(n) = smallest k such that n*k-1 as well as n+k are primes.
 * https://oeis.org/A085064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1953, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085064:  a(n) = smallest k such that n*k-1 and n+k are primes.
   The implementation performs a brute‑force search, checking each k
   until both expressions are prime using the provided is_prime helper. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    for (idx_t k = 1; ; ++k) {
        idx_t prod_minus = n * k - 1;
        if (is_prime(prod_minus) && is_prime(n + k))
            return (seq_t)k;
    }
}
