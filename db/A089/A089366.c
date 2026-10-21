/*
 * A089366  Smallest prime whose digital product is >= n.
 * https://oeis.org/A089366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   digit_prod_gt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest prime whose digit product is > n */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_prod_gt.h"

/* Helper: test if table contains prime <= capacity */
static bool is_prime_candidate(uint64_t n, uint64_t *primes, size_t len)
{
    /* trial division by known primes */
    for (size_t i = 0; i < len; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static storage for primes */
    static uint64_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;

    /* initialise */
    if (primes == NULL)
    {
        primes_cap = 16;
        primes_len = 0;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        /* first prime */
        primes[0] = 2;
        primes_len = 1;
    }

    /* candidate generator state */
    static uint64_t candidate = 3; /* next odd to test */

    size_t i = 0;
    for (;;)
    {
        /* Make sure we have prime[i] available */
        if (i >= primes_len)
        {
            /* find next prime and add to list */
            while (!is_prime_candidate(candidate, primes, primes_len))
                candidate += 2;                  /* skip evens */

            if (primes_len == primes_cap)
            {
                primes_cap *= 2;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = candidate;
            candidate += 2; /* next odd candidate */
        }

        uint64_t p = primes[i];
        if (digit_prod_gt(p, n))
            return (seq_t)p;  /* found the smallest prime */

        ++i; /* try next prime */
    }
}
