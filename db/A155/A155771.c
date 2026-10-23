/*
 * A155771  Numbers n such that 2*n^2+2*n-41 is a prime.
 * https://oeis.org/A155771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 119, u32 n <= 119, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th integer m such that 2*m^2 + 2*m – 41 is prime.
   The implementation incrementally searches candidates; it caches found
   terms so that calls with arbitrary n order remain efficient. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

#ifdef __cplusplus
#include <vector>
#endif

/* Return the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++: use a vector to store the terms. */
    static std::vector<seq_t> cache;
    static idx_t next_candidate = 1;

    while (cache.size() < n) {
        seq_t val = 2 * (seq_t)next_candidate * (seq_t)next_candidate
                    + 2 * (seq_t)next_candidate - 41;
        if (val >= 2 && is_prime_big(val))
            cache.push_back((seq_t)next_candidate);
        ++next_candidate;
    }
    return cache[(size_t)(n - 1)];
#else
    /* C: use a realloc‑based dynamic array. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;
    static idx_t next_candidate = 1;

    while (cache_sz < n) {
        seq_t val = 2 * (seq_t)next_candidate * (seq_t)next_candidate
                    + 2 * (seq_t)next_candidate - 41;
        if (val >= 2 && is_prime_big(val)) {
            if (cache_sz == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
#endif
}
