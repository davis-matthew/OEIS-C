/*
 * A141876  Primes congruent to 11 mod 19.
 * https://oeis.org/A141876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 11 mod 19. Computation is done by
 *            sieving the sequence of candidates 11, 30, 49, ... and testing
 *            each for primality with is_prime_small. The results are memoised
 *            so that the function can be called in any order
 * Range:     u8 n <= 3, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

static seq_t *cache = NULL;      /* memoised a(1), a(2), ... */
static size_t cache_cap = 0;     /* allocated capacity of cache      */
static size_t cache_len = 0;     /* number of terms stored so far   */
static uint64_t next_candidate = 11;   /* next number congruent to 11 mod 19 */

/* Ensure enough space in the cache for at least 'n' elements (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_len < (size_t)n) {
        /* Find next prime in the progression 11 mod 19 */
        while (!is_prime_small((seq_t)next_candidate)) {
            next_candidate += 19;
        }
        /* Store the prime */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)next_candidate;
        /* Advance to next candidate */
        next_candidate += 19;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n==0 is undefined but we can return 0 */
    if (n == 0)
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
