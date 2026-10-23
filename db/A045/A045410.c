/*
 * A045410  Primes congruent to {3, 5} mod 6.
 * https://oeis.org/A045410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045410: primes congruent to 3 or 5 mod 6 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t k_next = 1;          /* candidate of form 6*k-1 */

    /* Initialise cache with the first term (3) */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache_len = 1;
    }

    /* Extend cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t cand = (seq_t)6 * k_next - (seq_t)1;      /* 6*k-1 */
        if (is_prime((idx_t)cand)) {
            if (cache_len == cache_cap) {               /* grow array */
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = cand;
        }
        k_next++;
    }

    return cache[n - 1];
}
