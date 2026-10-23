/*
 * A134323  a(n) = Legendre(-3, prime(n)).
 * https://oeis.org/A134323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   Legendre(-3,prime(n)) = -1 if prime(n)=2 or ≡5(mod6), +1 if
 *            prime(n)≡1(mod6), 0 if prime(n)=3 (prime(1)=2, prime(2)=3, ...)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    /* Ensure n is within the valid range (>=1).  Undefined behavior for n<1 is acceptable. */
    uint32_t p = get_prime(n);    /* k‑th prime (1‑based) */
    if (p == 2u)
        return (seq_t)-1;
    if (p == 3u)
        return (seq_t)0;
    return (p % 6u == 1u) ? (seq_t)1 : (seq_t)-1;   /* p % 6 is 1 or 5 for all other primes */
}
