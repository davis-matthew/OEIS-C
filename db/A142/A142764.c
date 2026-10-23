/*
 * A142764  Primes congruent to 37 mod 59.
 * https://oeis.org/A142764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 37 mod 59
 * Range:     u8 n <= 1, u16 n <= 81, u32 n <= 100, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  // undefined; safe fallback

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static uint64_t next_candidate = 37;

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        cache[0] = 0;  /* unused slot for 1‑based indexing */
    }

    /* Expand cache until we have the nth term */
    while (cache_size < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;
        if (is_prime_big(cand)) {
            cache_size++;
            if (cache_size > cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache,
                                         (cache_cap + 1) * sizeof(seq_t));
            }
            cache[cache_size] = cand;
        }
        next_candidate += 59;
    }

    return cache[(size_t)n];
}
