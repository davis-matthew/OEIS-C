/*
 * A165440  Primes p such that 3+2*(sum of digits of p) is not a prime.
 * https://oeis.org/A165440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th element of sequence A165440:
   primes p such that 3 + 2 * (sum of decimal digits of p) is NOT a prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 2;   /* next integer to examine */

    /* generate terms until we have the n-th one */
    while (cache_len < n) {
        /* find next prime */
        while (!is_prime_big(next_candidate))
            ++next_candidate;

        /* test 3 + 2 * digit_sum(next_candidate) */
        seq_t dsum = digit_sum(next_candidate);
        seq_t test_val = 3 + 2 * dsum;

        /* store if test_val is not prime */
        if (!is_prime_big(test_val)) {
            /* ensure space in cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_ptr) exit(1);    /* allocation failure */
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }

        /* advance to next candidate: skip even numbers after 2 */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return cache[n - 1];
}
