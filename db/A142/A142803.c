/*
 * A142803  Primes congruent to 5 mod 61.
 * https://oeis.org/A142803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that is ≡ 5 (mod 61)
 * Range:     u8 n <= 2, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Handle indices below offset – results are undefined, but returning 0 is safe */
    if (n < 1) return (seq_t)0;

    /* Static cache of found primes of the form 61k+5 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 0;          /* index k for 61k+5; start at 0 */

    /* Grow cache array if needed.  Ensure we are able to store at least (n) primes. */
    if (cache_len < n) {
        /* Simple strategy: double capacity until enough space */
        size_t required = n;
        if (cache_cap < required) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < required) new_cap *= 2;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Extend the cache by checking successive candidates 61k+5 */
        while (cache_len < n) {
            uint64_t candidate = 61ULL * next_k + 5ULL;   /* candidate fits in 64 bits */
            if (is_prime(candidate))
                cache[cache_len++] = (seq_t)candidate;
            ++next_k;
        }
    }
    return cache[n - 1];
}
