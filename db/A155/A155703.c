/*
 * A155703  Primes p such that 2*p^2 + 16*p + 23 is also prime.
 * https://oeis.org/A155703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 491, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth prime p such that 2p^2 + 16p + 23 is also prime. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static uint64_t cand = 2; /* next integer to test */

    while (cache_size < n) {
        if (is_prime(cand)) {
            seq_t val = 2 * (seq_t)cand * cand + 16 * (seq_t)cand + 23;
            if (is_prime((idx_t)val)) {
                grow_seq(&cache, &cache_cap, cache_size + 1);
                cache[cache_size++] = (seq_t)cand;
            }
        }
        ++cand;
    }

    return cache[n - 1];
}
