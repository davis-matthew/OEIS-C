/*
 * A095959  a(n) = n-th prime modulo 30.
 * https://oeis.org/A095959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (n-th prime) mod 30 */
/* This function generates prime numbers on demand using trial division
 * and keeps them in a static array.  Each call returns the n‑th prime
 * reduced modulo 30.  The algorithm is O(n²) in the worst case but
 * performs well for the required input sizes (up to about 10 000). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state across calls */
    static uint64_t *primes = NULL;   /* array of found primes          */
    static size_t prime_cnt = 0;      /* number of primes stored        */
    static size_t primes_cap = 0;     /* current capacity of primes     */
    static uint64_t next_candidate = 2; /* next integer to test for prime */

    /* if we already have the n-th prime, return it modulo 30 */
    if (n <= prime_cnt)
        return (seq_t)(primes[n - 1] % 30);

    /* grow the primes array until we have at least n primes */
    while (prime_cnt < (size_t)n)
    {
        uint64_t p = next_candidate;

        /* primality test using previously found primes */
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t q = primes[i];
            if (q * q > p)
                break;
            if (p % q == 0)
            {
                is_prime = 0;
                break;
            }
        }

        if (is_prime)
        {
            /* ensure space for the new prime */
            if (prime_cnt >= primes_cap)
            {
                size_t newcap = primes_cap ? primes_cap * 2 : 128;
                uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
                if (!tmp)
                    abort();           /* allocation failed */
                primes = tmp;
                primes_cap = newcap;
            }
            primes[prime_cnt++] = p;
        }

        /* advance candidate: skip even numbers after 2 */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    /* now we have at least n primes */
    return (seq_t)(primes[n - 1] % 30);
}
