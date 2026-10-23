/*
 * A139049  a(n) = prime(n) + 6.
 * https://oeis.org/A139049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) + 6 (OEIS A139049)
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static uint64_t *prime_cache = NULL; /* array of found primes   */
static size_t prime_cap    = 0;      /* capacity of prime_cache */
static size_t prime_count  = 0;      /* number of primes stored */

/* Grow the prime cache array to at least new_cap entries. */
static void grow_cache(size_t new_cap)
{
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (tmp == NULL)
        abort();                       /* allocation failure */
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* Ensure that at least n primes are stored in prime_cache. */
static void cache_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;                      /* already enough */

    /* Grow capacity if necessary */
    if (prime_cap < prime_count + 16)    /* mild growth factor */
        grow_cache((prime_cap == 0) ? 16 : prime_cap * 2);

    uint64_t candidate;
    if (prime_count == 0)
        candidate = 2;
    else
        candidate = prime_cache[prime_count - 1] + 1;

    while (prime_count < (size_t)n) {
        /* Test candidate for primality using already known primes */
        uint64_t limit = (uint64_t)floor_sqrt((seq_t)candidate);
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_cache[i];
            if (p > limit)
                break;
            if ((seq_t)candidate % (seq_t)p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap)
                grow_cache(prime_cap * 2);
            prime_cache[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* Undefined for 0, harmless */
    cache_primes(n);
    return (seq_t)prime_cache[n - 1] + (seq_t)6;
}
