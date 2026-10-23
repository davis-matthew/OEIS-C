/*
 * A142143  Primes congruent to 34 mod 37.
 * https://oeis.org/A142143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes congruent to 34 modulo 37  
   Computed by generating candidates 34 + 37*k and testing each for primality.
   The function is thread‑local to the calling process; results are memoised
   so that subsequent calls with larger indices are fast. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* cached prime values */
static size_t cache_size = 0;        /* number of cached values */
static size_t cache_cap = 0;         /* capacity of the cache */

static uint64_t next_k = 0;          /* next candidate index to test */

/* resize the cache if required */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
    cache = tmp;
    cache_cap = new_cap;
}

static bool is_candidate_prime(uint64_t p)
{
    /* the provided is_prime works for any idx_t (here uint64_t). */
    return is_prime(p);
}

seq_t a(idx_t n)
{
    /* ensure the cache is large enough */
    if (n == 0)
        return 0;                     /* not used – offset 1 */

    while (cache_size < (size_t)n) {
        if (next_k == 0)
            next_k = 1;               /* start with the first candidate */

        uint64_t candidate = 34ULL + 37ULL * next_k;
        ++next_k;

        if (is_candidate_prime(candidate)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)candidate;
        }
    }
    return cache[n - 1];
}
