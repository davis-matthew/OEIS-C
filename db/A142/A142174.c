/*
 * A142174  Primes congruent to 25 mod 39.
 * https://oeis.org/A142174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 263, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching for primes congruent to 25 mod 39 */
#include <stddef.h>          /* size_t */
#include <stdbool.h>         /* bool */
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* static cache of found primes */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_candidate = 25; /* first number ≡25 (mod 39) */

    if (n == 0) return 0; /* offset is 1; return 0 for invalid index */

    /* extend cache until it contains at least n primes */
    while (cache_len < (size_t)n) {
        idx_t candidate = next_candidate;

        /* find the next prime ≡25 (mod 39) */
        while (candidate < 2 || !is_prime(candidate)) {
            candidate += 39;
        }

        /* store the prime */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)candidate;

        /* set candidate for next search */
        next_candidate = candidate + 39;
    }

    return cache[n - 1];
}
