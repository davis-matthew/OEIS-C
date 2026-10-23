/*
 * A048125  Becomes prime or 4 after exactly 3 iterations of f(x) = sum of
 *          prime factors of x.
 * https://oeis.org/A048125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048125: numbers that become prime or 4 after exactly 3 iterations
   of f(x) = sum of prime factors with multiplicity.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return the sum of prime factors of n (with multiplicity).  */
static uint64_t sum_prime_factors(uint64_t n)
{
    uint64_t sum = 0;
    /* factor 2 */
    while ((n & 1) == 0)
    {
        sum += 2;
        n >>= 1;
    }
    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2)
    {
        while (n % p == 0)
        {
            sum += p;
            n /= p;
        }
    }
    if (n > 1)
        sum += n;
    return sum;
}

/* Primality test for 64‑bit integers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t p = 3; p <= n / p; p += 2)
        if (n % p == 0) return false;
    return true;
}

/* Compute term a(n). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static uint64_t next_candidate = 2; /* start searching from 2 */

    /* Ensure the cache is large enough. */
    if (cache_size < (size_t)n)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;          /* double until enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have n of them. */
    while (cache_size < (size_t)n)
    {
        while (1)
        {
            uint64_t t1 = sum_prime_factors(next_candidate);
            if (t1 == 4 || is_prime_u64(t1))
            {
                ++next_candidate;
                continue;           /* becomes prime or 4 after 1 step */
            }

            uint64_t t2 = sum_prime_factors(t1);
            if (t2 == 4 || is_prime_u64(t2))
            {
                ++next_candidate;
                continue;           /* becomes prime or 4 after 2 steps */
            }

            uint64_t t3 = sum_prime_factors(t2);
            if (t3 == 4 || is_prime_u64(t3))
            {
                cache[cache_size++] = (seq_t)next_candidate;
                ++next_candidate;   /* next search starts after found number */
                break;
            }

            ++next_candidate;       /* candidate does not satisfy condition */
        }
    }

    return cache[n - 1];
}
