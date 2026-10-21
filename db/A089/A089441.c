/*
 * A089441  Primes p such that 16*p+17 is a prime.
 * https://oeis.org/A089441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: primes p such that 16*p + 17 is prime.
 * Calculation: iterative search of primes, testing 16*p+17 for primality
 * using the provided helpers.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

/* This function returns the n-th prime p with 16*p+17 prime. */
seq_t a(idx_t n)
{
    /* Static state to support non‑monotonic n */
    static uint64_t last_checked = 1ULL;   /* start before the first prime */
    static seq_t *cache      = NULL;
    static size_t cache_cap   = 0;
    static size_t cache_len   = 0;

    if (n == 0)        /* under the offset – return 0 */
        return (seq_t)0;

    /* grow cache until we have at least n elements */
    while (cache_len < n)
    {
        uint64_t p = next_prime(last_checked);
        last_checked = p;

        seq_t val = (seq_t)p * 16ULL + (seq_t)17ULL;
        if (is_prime_big(val))
        {
            if (cache_len == cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *newp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!newp)
                    abort();  /* out of memory */
                cache = newp;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
