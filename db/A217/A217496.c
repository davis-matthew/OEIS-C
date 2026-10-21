/*
 * A217496  Primes of the form 2*n^2 + 50*n + 23.
 * https://oeis.org/A217496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime of the form 2*k^2 + 50*k + 23. The
 *            function searches successive k (starting at 0), tests each value
 *            for primality, and caches the primes found
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 34, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* static cache of primes and the next k to examine */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_k = 0;

    while (cache_len < (size_t)n) {
        seq_t candidate = (seq_t)2 * (seq_t)next_k * (seq_t)next_k
                         + (seq_t)50 * (seq_t)next_k
                         + (seq_t)23;
        if (is_prime_big(candidate)) {
            append_candidate(&cache, &cache_len, &cache_cap, candidate);
        }
        ++next_k;
    }

    return cache[n - 1];
}
