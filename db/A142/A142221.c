/*
 * A142221  Primes congruent to 24 mod 41.
 * https://oeis.org/A142221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 134, u32 n <= 135, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 24 (mod 41) */
# include <stdlib.h>
# include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of primes found so far */
    static seq_t *cache = NULL;
    static size_t cache_cnt = 0, cache_cap = 0;
    static idx_t next_candidate = 24;          /* first number ≡ 24 (mod 41) */

    while (cache_cnt < n) {
        /* Test current candidate for primality */
        seq_t cand = (seq_t)next_candidate;
        if (is_prime_big(cand)) {
            if (cache_cnt >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_cnt++] = cand;
        }
        /* Advance to next number ≡ 24 (mod 41) */
        next_candidate += 41;
    }
    return cache[n - 1];
}
