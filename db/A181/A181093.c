/*
 * A181093  p*(p+2)/3 where p and p+4 are primes.
 * https://oeis.org/A181093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*(p+2)/3 where p and p+4 are primes
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_size = 0;
    static uint64_t next_candidate = 3;   /* first candidate p */

    if (cache == NULL) {
        cache_capacity = 16;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    while (cache_size < (size_t)n) {
        uint64_t p = next_candidate;

        if (is_prime_small(p) && is_prime_small(p + 4)) {
            /* term = p*(p+2)/3  */
            seq_t term = (seq_t)p * ((seq_t)p + 2) / 3;

            if (cache_size == cache_capacity) {
                cache_capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_size++] = term;
        }

        /* advance to the next candidate (all 1 mod 6 numbers, except the first step) */
        if (next_candidate == 3)
            next_candidate = 7;
        else
            next_candidate += 6;
    }

    return cache[n - 1];
}
