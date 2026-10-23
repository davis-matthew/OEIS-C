/*
 * A100629  a(n) = 2^(5*prime(n) + 1).
 * https://oeis.org/A100629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(5*prime(n)+1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/pow_seq.h"

/* Compute the n-th prime (OEIS offset 1).  Cache results for repeated use. */
static seq_t nth_prime(idx_t n)
{
    /* static cache */
    static idx_t primes_count = 0;            /* number of primes found         */
    static uint32_t *primes = NULL;           /* array of primes found so far   */
    static size_t primes_cap   = 0;           /* allocated capacity             */

    /* Grow the cache if needed */
    while (primes_count < n)
    {
        /* Grow capacity if needed */
        if (primes_count + 16 > primes_cap)
        {
            size_t new_cap = primes_cap ? primes_cap * 2 : 128;
            uint32_t *new_arr = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
            if (!new_arr) abort();          /* OOM – terminate narrowly */
            primes = new_arr;
            primes_cap = new_cap;
        }

        /* Find next prime */
        uint32_t candidate = (primes_count == 0) ? 2u : primes[primes_count - 1] + 1u;
        for (;;)
        {
            if (is_prime((uint64_t)candidate))
            {
                primes[primes_count++] = candidate;
                break;
            }
            ++candidate;
        }
    }
    return (seq_t)primes[n - 1];
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1 – return 0 */
    seq_t p   = nth_prime(n);
    idx_t exp = 5 * (idx_t)p + 1;      /* 5*prime + 1  */
    return pow_seq((seq_t)2, exp);
}
