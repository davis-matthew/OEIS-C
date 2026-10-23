/*
 * A140552  Primes p such that neither p - 34 nor p + 34 is prime.
 * https://oeis.org/A140552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140552: primes p such that neither p-34 nor p+34 is prime */

/* External utilities */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of recorded primes */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Static generator state -- next integer to test */
    static idx_t cand = 2;

    /* Grow the cache until it contains n entries */
    while (cache_len < n) {
        /* Find the next prime number */
        while (!is_prime(cand)) {
            cand += (cand == 2 ? 1 : 2);   /* skip even numbers after 2 */
        }

        /* Check the condition for the current prime cand */
        bool good = true;
        if (cand > 34 && is_prime(cand - 34)) {
            good = false;
        }
        if (is_prime(cand + 34)) {
            good = false;
        }

        /* Store the prime if it satisfies the condition */
        if (good) {
            if (cache_len == cache_cap) {
                /* Grow capacity */
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)cand;
        }

        /* Advance to the next candidate (odd after 2) */
        cand += (cand == 2 ? 1 : 2);
    }

    return cache[n - 1];   /* a(1) is the first element */
}
