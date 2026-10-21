/*
 * A067463  Primes p such that p+sigma(p+1) is prime.
 * https://oeis.org/A067463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   primes p such that p + sigma(p + 1) is prime
 * Range:     u8 n <= 12, u16 n <= 526, u32 n <= 527, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The logic below computes successive primes that satisfy the
   condition.  A static cache of found primes is used to allow
   calls in any order.  The routine is efficient for n up to
   at least 10 000. */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* static cache of found primes (type seq_t) */
static seq_t *prime_cache = NULL;
static size_t prime_cache_len = 0;
static size_t prime_cache_cap = 0;

/* last prime examined for the next search */
static uint64_t last_prime_searched = 2;

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= prime_cache_cap)
        return;
    if (prime_cache_cap == 0)
        prime_cache_cap = 8;
    while (prime_cache_cap < needed)
        prime_cache_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, prime_cache_cap * sizeof(seq_t));
    prime_cache = tmp;
}

/* a(n): nth prime with the required property.  a(1) is the first
   such prime. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n < 1, but safe */

    /* return precomputed value if available */
    if ((size_t)n <= prime_cache_len)
        return prime_cache[n - 1];

    /* generate more primes until we have at least n of them */
    while (prime_cache_len < (size_t)n)
    {
        /* find the next prime after the last one examined */
        uint64_t cand = next_prime(last_prime_searched + 1U);
        last_prime_searched = cand;

        /* compute sigma(cand + 1) */
        idx_t m = (idx_t)cand + 1ULL;
        seq_t sigma = sigma_of(m);

        /* test whether cand + sigma is prime */
        seq_t sum = (seq_t)cand + sigma;
        if (is_prime_big(sum))
        {
            /* store the candidate */
            ensure_cache_capacity(prime_cache_len + 1);
            prime_cache[prime_cache_len++] = (seq_t)cand;
        }
    }

    return prime_cache[n - 1];
}
