/*
 * A142226  Primes congruent to 29 mod 41.
 * https://oeis.org/A142226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 29 (mod 41)
 * Range:     u8 n <= 2, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0)
        return 0;   /* value not defined for n < 1 */

    /* Static cache of found primes and the next candidate to test. */
    static seq_t *cache     = NULL;  /* array of primes found so far */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t   cache_cnt = 0;    /* number of primes stored */
    static seq_t   next_candidate = 0;  /* candidate to test next */

    /* Initialise on first call */
    if (next_candidate == (seq_t)0)
        next_candidate = (seq_t)29;

    /* Generate more primes until we have at least n of them */
    while (cache_cnt < n) {
        /* If the candidate is prime, store it */
        if (is_prime_small(next_candidate)) {
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                if (new_cap < cache_cnt + 1)
                    new_cap = cache_cnt + 1;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = next_candidate;
        }
        /* Move to the next number that is still ≡ 29 (mod 41) */
        next_candidate += (seq_t)41;
    }

    /* Return the (n-1)-th element of the cache (0‑based index) */
    return cache[n - 1];
}
