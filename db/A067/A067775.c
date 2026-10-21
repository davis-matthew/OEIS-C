/*
 * A067775  Primes p such that p + 4 is composite.
 * https://oeis.org/A067775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p for which p + 4 is composite
 * Range:     u8 n <= 38, u16 n <= 1026, u32 n <= 1026, u64 n <= 1026,
 *            u128 n <= 1026, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by an incremental prime test using is_prime(). */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_prime = 0; /* last prime examined */

    while (cache_len < n) {
        /* Find the next prime >= last_prime + 1 */
        idx_t cand = last_prime + 1;
        while (!is_prime(cand))
            cand++;

        /* cand is a prime; check if cand + 4 is composite */
        if (!is_prime(cand + 4)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* out of memory: undefined behavior for this sequence */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
        last_prime = cand;
    }

    return cache[n - 1];
}
