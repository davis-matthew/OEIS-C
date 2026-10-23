/*
 * A125821  Numbers k for which 8*k+5 and 8*k+7 are twin primes.
 * https://oeis.org/A125821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 240, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k for which 8k+5 and 8k+7 are twin primes. */
/* The function scans multiples of 3, testing primality of the two
   candidates, and caches the results for subsequent calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k = 3;      /* first candidate k (k = 3) */

    if (n < 1) return 0;  /* undefined for n < 1; return 0 as a safe default */

    /* Ensure the cache contains at least n terms */
    while (cache_len < (size_t)n) {
        uint64_t k   = next_k;
        uint64_t p1  = 8 * k + 5;
        uint64_t p2  = 8 * k + 7;

        if (is_prime_small((seq_t)p1) && is_prime_small((seq_t)p2)) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation error – abort */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)k;
            cache_len++;
        }
        next_k += 3;   /* advance to next multiple of 3 */
    }

    return cache[(size_t)n - 1];
}
