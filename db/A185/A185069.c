/*
 * A185069  Primes of the form k*6^m +1 with k prime and m an integer.
 * https://oeis.org/A185069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 450, u32 n <= 318, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form k*6^m + 1 with k prime */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Static cache of generated terms */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next prime candidate to test; starts with the first odd prime */
static seq_t next_candidate = 3;

/* Test if a prime p satisfies the defining condition */
static bool qualifies(seq_t p)
{
    seq_t q = p - 1;
    while (q % 6 == 0)
        q /= 6;
    return is_prime_big(q);
}

/* Return the n-th term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n<1, return 0 for safety */
        return 0;

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Find the next prime candidate */
        while (!is_prime_big(next_candidate)) {
            if (next_candidate == 3)
                next_candidate = 5;
            else
                next_candidate += 2;
        }

        /* If the prime satisfies the property, store it */
        if (qualifies(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                seq_cache = (seq_t *)realloc(seq_cache,
                                             cache_cap * sizeof(seq_t));
            }
            seq_cache[cache_len++] = next_candidate;
        }

        /* Advance to the next candidate */
        if (next_candidate == 3)
            next_candidate = 5;
        else
            next_candidate += 2;
    }

    return seq_cache[(size_t)n - 1];
}
