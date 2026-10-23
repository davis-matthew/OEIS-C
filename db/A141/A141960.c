/*
 * A141960  Primes congruent to 19 mod 27.
 * https://oeis.org/A141960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 19 (mod 27)
 * Range:     u8 n <= 4, u16 n <= 350, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Lazy memoisation of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 0;  /* last prime found in the sequence */

    if (n == 0) return 0;  /* offset starts at 1, behaviour for n<1 irrelevant */

    /* Extend the cache until it contains the nth element */
    while (cache_size < (size_t)n) {
        uint64_t cand;
        if (last_checked == 0)
            cand = 19;                 /* first prime in the progression */
        else
            cand = last_checked + 27;  /* next candidate in the progression */

        /* Search for the next prime candidate */
        while (!is_prime(cand))
            cand += 27;

        /* Grow the cache if necessary */
        if (cache_size == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = cache
                          ? (seq_t *)realloc(cache, cache_cap * sizeof(seq_t))
                          : (seq_t *)malloc(cache_cap * sizeof(seq_t));
            cache = tmp;
        }

        cache[cache_size++] = (seq_t)cand;
        last_checked = cand;
    }

    return cache[n - 1];
}
