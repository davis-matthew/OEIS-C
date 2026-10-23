/*
 * A142392  Primes congruent to 41 mod 47.
 * https://oeis.org/A142392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 41 modulo 47.
   a(n) returns the n‑th prime p such that p % 47 == 41. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t *prime_cache = NULL;   /* array of found primes */
static size_t cache_len = 0;        /* number of primes stored */
static size_t cache_cap = 0;        /* capacity of prime_cache */

/* Ensure that prime_cache contains at least 'need' primes. */
static void ensure_prime_cache(idx_t need)
{
    if (cache_len >= (size_t)need)
        return;

    /* Start searching from the next number congruent to 41 mod 47. */
    idx_t cand = (cache_len == 0) ? 41
                                  : prime_cache[cache_len - 1] + 47;

    while (cache_len < (size_t)need) {
        /* Advance cand until it is prime. */
        while (!is_prime(cand))
            cand += 47;

        /* Grow the cache if necessary. */
        if (cache_len == cache_cap) {
            size_t newcap = (cache_cap == 0) ? 16 : cache_cap * 2;
            prime_cache = (idx_t*)realloc(prime_cache, newcap * sizeof(idx_t));
            cache_cap = newcap;
        }

        /* Store the found prime. */
        prime_cache[cache_len++] = cand;
        cand += 47; /* next candidate */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */
    ensure_prime_cache(n);
    return (seq_t)prime_cache[n - 1];
}
