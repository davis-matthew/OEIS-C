/*
 * A142060  Primes congruent to 19 mod 33.
 * https://oeis.org/A142060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 259, u32 n <= 290, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 19 (mod 33) */
/* a(n) = n‑th prime p such that p ≡ 19 (mod 33) */

#include <stddef.h>
#include <stdbool.h>
/* Utility functions used by the implementation */
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Cache of already found terms */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of cached elements   */
    static size_t cap   = 0;   /* capacity of the cache       */
    /* Next candidate in the progression 19, 52, 85, …   */
    static seq_t next_candidate = 19;
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        next_candidate = 19;
    }

    /* Generate primes until we have the n‑th term */
    while (len < n) {
        if (is_prime_big(next_candidate)) {
            grow_seq(&cache, &cap, len + 1);
            cache[len++] = next_candidate;
        }
        next_candidate += 33;
    }

    return cache[n - 1];
}
