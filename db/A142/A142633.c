/*
 * A142633  Primes congruent to 46 mod 55.
 * https://oeis.org/A142633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 46 modulo 55
 * Range:     u8 n <= 2, u16 n <= 155, u32 n <= 156, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static uint64_t next_candidate = 46;
    static bool initialized = false;

    if (!initialized) {
        cache_cap = 256;
        cache = alloc_seq_array(cache_cap);
        cache_size = 0;
        next_candidate = 46;
        initialized = true;
    }

    while (cache_size < (size_t)n) {
        if (is_prime_big((seq_t)next_candidate)) {
            if (cache_size == cache_cap) {
                grow_seq(&cache, &cache_cap, cache_size + 1);
            }
            cache[cache_size] = (seq_t)next_candidate;
            ++cache_size;
        }
        next_candidate += 55;
    }

    return cache[(size_t)(n - 1)];
}
