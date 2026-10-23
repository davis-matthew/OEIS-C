/*
 * A154981  Primes with nonprime smallest digit.
 * https://oeis.org/A154981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime whose smallest decimal digit is nonprime
 *            (0,1,4,6,8,9)
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_smallest_digit_nonprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_smallest_digit_nonprime.h"

/*
 * a(n) returns the n-th prime whose smallest decimal digit is nonprime.
 * The function is fully lazy and memoized: subsequent calls reuse previously
 * computed primes and cache entries.  No assumptions are made about call order.
 */
seq_t a(idx_t n)
{
    /* static state for caching */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_candidate = 2;                 /* next number to test */
    static idx_t *prime_list = NULL;                /* array of primes found so far */
    static size_t prime_count = 0;
    static size_t prime_cap   = 0;

    if (n == 0) return 0;          /* offset starts at 1 */

    /* Expand cache capacity lazily */
    if (cache == NULL) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    /* Expand prime list capacity lazily */
    if (prime_list == NULL) {
        prime_cap = 128;
        prime_list = (idx_t *)malloc(prime_cap * sizeof(idx_t));
        prime_count = 0;
    }

    /* Generate primes and fill the cache until we have at least n entries */
    while (cache_size < n) {
        /* Check if next_candidate is prime */
        bool is_prime = true;
        idx_t candidate = next_candidate;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p > candidate / p) break;  /* p*p would overflow or exceed sqrt(candidate) */
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the prime in the list */
            if (prime_count >= prime_cap) {
                prime_cap *= 2;
                prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
            }
            prime_list[prime_count++] = candidate;

            /* If the prime satisfies the condition, cache it */
            if (is_smallest_digit_nonprime((seq_t)candidate)) {
                if (cache_size >= cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size++] = (seq_t)candidate;
            }
        }

        /* Advance to the next candidate (skip even numbers after 2) */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return cache[n - 1];
}
