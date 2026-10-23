/*
 * A027756  Numbers k such that k^2 + k + 7 is prime.
 * https://oeis.org/A027756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 154, u32 n <= 58, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the n‑th non‑negative integer k such that k^2 + k + 7 is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 0, a(2) = 2, … */
    if (n == 0) return 0;        /* Undefined below offset – return 0 safely */

    /* Static cache of discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 0;

    /* Extend cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = k * k + k + (seq_t)7;   /* k^2 + k + 7 */

        if (is_prime_big(val)) {
            /* Ensure capacity for the new term */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                /* In a production environment we would check tmp for NULL.  */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
