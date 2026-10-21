/*
 * A053760  Smallest positive quadratic nonresidue modulo p, where p is the
 *          n-th prime.
 * https://oeis.org/A053760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A020649(prime(n)) (smallest positive quadratic nonresidue
 *            mod prime(n)) with a(1) = 2 (since p₁ = 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime64, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime64.h"
#include "../utils/powmod64.h"

/* Return the n-th term of OEIS A053760 */
seq_t a(idx_t n)
{
    /* Static state: generated prime table */
    static uint64_t *primes = NULL;          /* array of primes found so far */
    static size_t primes_capacity = 0;      /* current allocation size */
    static size_t primes_count = 0;         /* number of primes stored */
    static uint64_t next_candidate = 2;     /* next number to test for primality */

    /* Ensure we have at least n primes in the table */
    while (primes_count < n)
    {
        if (is_prime64(next_candidate))
        {
            /* Grow the prime array if necessary */
            if (primes_count >= primes_capacity)
            {
                size_t new_capacity = primes_capacity ? (primes_capacity << 1) : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_capacity * sizeof(uint64_t));
                if (!tmp) return (seq_t)0;            /* allocation failure – unlikely */
                primes = tmp;
                primes_capacity = new_capacity;
            }
            primes[primes_count++] = next_candidate;
        }
        /* advance candidate: skip even numbers after 2 */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    uint64_t prime = primes[n - 1];  /* prime(n), zero‑based index */

    /* Handle p = 2 separately */
    if (prime == 2)
        return (seq_t)2;

    /* Find the smallest a > 0 such that a is a quadratic nonresidue modulo prime */
    for (uint64_t a = 2; ; ++a)
    {
        uint64_t leg = powmod64(a, (prime - 1U) / 2U, prime);
        /* Legendre symbol ≡ -1 (mod prime) is represented by prime-1 */
        if (leg == prime - 1U)
            return (seq_t)a;
    }
}
