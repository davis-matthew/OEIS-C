/*
 * A062713  Numbers k such that the sum of the digits of k is a prime factor
 *          of k.
 * https://oeis.org/A062713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k such that the sum of the digits of k is a prime factor of k */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache of pre‑computed terms */
    static seq_t *cache      = NULL;
    static size_t   cache_cap = 0;
    static size_t   cache_len = 0;
    static seq_t   next_candidate = (seq_t)2;
    static bool    initialized   = false;

    if (!initialized) {
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;
        seq_t sum  = digit_sum(cand);

        if (is_prime_small(sum) && (cand % sum == 0)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        next_candidate = cand + (seq_t)1;
    }

    /* n is 1‑based, cache is 0‑based */
    return cache[n - 1];
}
