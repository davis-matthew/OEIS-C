/*
 * A215848  Primes > 3.
 * https://oeis.org/A215848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000040(n+2) (the (n+2)-th prime)
 * Range:     u8 n <= 52, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* static cache of primes, 1‑based: prime_list[0]=2, prime_list[1]=3, ... */
static uint64_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_cnt   = 0;
static uint64_t next_candidate = 2;

/* Extend the prime cache until there are at least 'target' primes stored. */
static void extend_primes(size_t target)
{
    while (prime_cnt < target) {
        /* Find the next prime. */
        while (!is_prime_small(next_candidate))
            ++next_candidate;

        /* Grow the array if necessary. */
        if (prime_cnt >= prime_cap) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 16;
            prime_list = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
            if (!prime_list) abort(); /* allocation failure */
            prime_cap = new_cap;
        }

        /* Store the new prime. */
        prime_list[prime_cnt++] = next_candidate;
        ++next_candidate;
    }
}

/* sequence A215848: primes > 3, with a(1)=5. */
seq_t a(idx_t n)
{
    size_t target = (size_t)n + 2;          /* need this many primes (1‑based) */
    if (prime_cnt < target)
        extend_primes(target);

    /* prime number at position n+2 (1‑based). */
    return (seq_t)prime_list[n + 1];
}
