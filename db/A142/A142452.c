/*
 * A142452  Primes congruent to 45 mod 49.
 * https://oeis.org/A142452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 45 modulo 49 (sequence
 *            A142452)
 * Range:     u8 n <= 1, u16 n <= 150, u32 n <= 97, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provided generic prime test */

/* Cached results of a(n). */
static seq_t *cache   = NULL;   /* array of found primes */
static size_t cache_len = 0;    /* how many primes stored */
static size_t cache_cap = 0;    /* allocated capacity */

/* Next candidate number of the form 49*k + 45 to test. */
static uint64_t next_candidate = 45U;

/* Compute a(n).  The sequence is 1‑based. */
seq_t a(idx_t n)
{
    /* Grow the cache until it contains at least n primes. */
    while (cache_len < (size_t)n) {
        /* Test the current candidate for primality. */
        if (is_prime_big((seq_t)next_candidate)) {
            /* Ensure the cache has space. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    /* Allocation failure: terminate. */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        /* Advance to the next number of the form 49*k + 45. */
        next_candidate += 49;
    }

    return cache[n - 1];
}
