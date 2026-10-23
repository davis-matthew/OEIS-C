/*
 * A031379  a(n) = prime(5*n - 2).
 * https://oeis.org/A031379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = prime(5*n-2) by successive next_prime calls. */
#include <stdint.h>
#include "../utils/next_prime.h"   /* declares uint64_t next_prime(uint64_t start); */

seq_t a(idx_t n)
{
    /* the required index of the prime (1‑based) */
    idx_t k = 5 * n - 2;
    if (k == 0)          /* not used for n≥1, but keeps the function safe */
        return 0;

    uint64_t prime = 2;  /* the 1st prime */
    idx_t    count  = 1; /* number of primes found so far */

    if (k == 1)
        return (seq_t)prime;

    /* repeatedly find the next prime until the k‑th prime is reached */
    while (count < k) {
        prime = next_prime(prime);   /* next prime > current prime */
        ++count;
    }

    return (seq_t)prime;
}
