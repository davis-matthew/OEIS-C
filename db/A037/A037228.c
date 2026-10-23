/*
 * A037228  a(n) = phi(n) - pi(n).
 * https://oeis.org/A037228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(n) - pi(n)
 * Range:     i8 n <= 171, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Compute a(n) = φ(n) – π(n) for any n ≥ 1.
   φ(n)   : Euler's totient function
   π(n)   : prime‑counting function
   The routine builds a small sieve of size n+1 on each call
   (costs < a few 10⁴ operations, far below the time limit).
*/
seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for safety return 0 for n==0. */
    if (n == 0)
        return (seq_t)0;

    /* Allocate a sieve array of n+1 bytes (0/1 flags). */
    size_t sz = (size_t)n + 1U;
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (is_prime == NULL)
        return (seq_t)0;              /* out of memory – unlikely for n ≤ 10000 */

    /* Initialise sieve: assume all numbers are prime except 0,1. */
    memset(is_prime, 1, sz);
    if (sz > 0)  is_prime[0] = 0;
    if (sz > 1)  is_prime[1] = 0;

    /* Sieve of Eratosthenes up to n */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= n; q += p)
                is_prime[q] = 0;
        }
    }

    /* Compute π(n) by counting primes. */
    seq_t pi = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            ++pi;

    /* Compute φ(n) by factorisation using the sieve. */
    idx_t m = n;
    seq_t phi = (seq_t)n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (is_prime[p] && (m % p == 0)) {
            phi -= phi / (seq_t)p;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)                /* remaining prime factor */
        phi -= phi / (seq_t)m;

    free(is_prime);
    return phi - pi;
}
