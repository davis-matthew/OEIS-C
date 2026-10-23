/*
 * A141895  Primes congruent to 16 mod 21.
 * https://oeis.org/A141895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 16 modulo 21, i.e. primes p
 *            such that p ≡ 16 (mod 21)
 * Range:     u8 n <= 3, u16 n <= 539, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"   /* is_prime(idx_t x) */

/* External integer types supplied by the build. */
extern seq_t a(idx_t n);

/* Memoized cache of found primes. */
static seq_t *prime_cache = NULL;   /* array of primes (seq_t) */
static idx_t  prime_count = 0;      /* number of primes stored */
static idx_t  prime_capacity = 0;   /* current allocated capacity */
static idx_t  next_k = 1;           /* k such that candidate = 16 + 21*k */

/* Ensure that at least 'min_capacity' slots are available. */
static void
ensure_capacity(idx_t min_capacity)
{
    if (prime_capacity >= min_capacity)
        return;
    idx_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
    if (new_cap < min_capacity)
        new_cap = min_capacity;
    seq_t *tmp = (seq_t *)realloc(prime_cache,
                                  (size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* allocation failed */
    prime_cache = tmp;
    prime_capacity = new_cap;
}

/* Return the n-th prime ≡ 16 (mod 21).  Indexing starts at 1. */
seq_t
a(idx_t n)
{
    if (n == 0)            /* undefined for n < 1 – never used */
        return (seq_t)0;

    /* If already computed, return cached value. */
    if (n <= prime_count)
        return prime_cache[n - 1];

    /* Generate primes until the cache holds at least n elements. */
    while (prime_count < n) {
        idx_t candidate = 16 + 21 * next_k;
        if (is_prime(candidate)) {
            ensure_capacity(prime_count + 1);
            prime_cache[prime_count] = (seq_t)candidate;
            prime_count++;
        }
        next_k++;
    }

    return prime_cache[n - 1];
}
