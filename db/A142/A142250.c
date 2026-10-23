/*
 * A142250  Primes congruent to 1 mod 43.
 * https://oeis.org/A142250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth prime ≡ 1 (mod 43) by incremental primality testing. */
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static idx_t last_candidate = 0;

    if (n == 0) return 0;  /* Undefined for offset 0, but guard anyway. */

    while (cache_len < n) {
        idx_t candidate;
        if (cache_len == 0) {
            candidate = 173;          // First prime ≡1 (mod 43)
        } else {
            candidate = last_candidate + 43;
        }

        /* Search forward for the next such prime. */
        while (!is_prime(candidate)) {
            candidate += 43;
        }

        /* Store the found prime. */
        ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[cache_len++] = (seq_t)candidate;
        last_candidate = candidate;
    }

    return cache[n - 1];
}
