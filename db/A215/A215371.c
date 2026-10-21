/*
 * A215371  Primes congruent to {0, 2, 3, 5, 6} mod 13.
 * https://oeis.org/A215371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = the n-th prime p such that p ≡ 0, 2, 3, 5, or 6 (mod 13)
 *   (with offset 1, so a(1) = 2)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Test whether a prime candidate matches the required residues */
static inline bool allowed_residue(idx_t p)
{
    if (p == 13ULL) return true;
    return (p % 13ULL == 2ULL ||
            p % 13ULL == 3ULL ||
            p % 13ULL == 5ULL ||
            p % 13ULL == 6ULL);
}

/* Static cache for the sequence. */
static seq_t *cache      = NULL; /* array of found primes */
static size_t cache_cap  = 0;     /* allocated capacity */
static size_t cache_len  = 0;     /* number of primes stored */
static idx_t next_candidate = 2ULL; /* candidate to test */

/* Grow the cache capacity if necessary. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) { /* allocation failed – keep old pointer */
        return;
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th prime with the specified residues. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1 – this case is unused */

    /* Ensure we have at least n values in the cache. */
    while (cache_len < (size_t)n) {
        /* Grow cache if full. */
        if (cache_len >= cache_cap) grow_cache();

        /* Search for the next prime satisfying the residue condition. */
        for (; ; ++next_candidate) {
            if (is_prime_small((uint64_t)next_candidate) &&
                allowed_residue(next_candidate)) {
                /* Store the prime. */
                cache[cache_len++] = (seq_t)next_candidate;
                /* Move candidate past this prime for the next call. */
                ++next_candidate;
                break;
            }
        }
    }

    return cache[n - 1];
}
