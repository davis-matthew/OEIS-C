/*
 * A142161  Primes congruent to 4 mod 39.
 * https://oeis.org/A142161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Computes the n-th prime that is congruent to 4 (mod 39)
 * Range:     u8 n <= 2, u16 n <= 272, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses an incremental search and a static cache for reuse of results. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of primes stored */
    static idx_t cache_cap = 0;   /* current capacity of cache */

    if (n == 0) {
        /* OEIS offset starts at 1; for n==0 we return 0 (undefined) */
        return 0;
    }

    /* grow cache until we have at least n entries */
    if (n > cache_len) {
        /* ensure at least n elements */
        idx_t needed = n;
        if (needed > cache_cap) {
            idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            while (new_cap < needed)
                new_cap *= 2;
            seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_ptr) {
                /* out of memory: cannot continue */
                return 0;
            }
            cache = new_ptr;
            cache_cap = new_cap;
        }

        /* start searching from the next candidate congruent to 4 mod 39 */
        uint64_t start_candidate;
        if (cache_len == 0)
            start_candidate = 43;          /* first prime meeting the condition */
        else
            start_candidate = (uint64_t)cache[cache_len - 1] + 39ULL;

        uint64_t candidate = start_candidate;
        while (cache_len < n) {
            if (is_prime(candidate)) {
                cache[cache_len++] = (seq_t)candidate;
            }
            candidate += 39ULL;  /* advance to next possible candidate */
        }
    }

    /* n-1 is a valid index because n >= 1 */
    return cache[n - 1];
}
