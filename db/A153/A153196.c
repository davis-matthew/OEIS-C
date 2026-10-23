/*
 * A153196  Numbers k such that 6*k+5 and 6*k+7 are twin primes.
 * https://oeis.org/A153196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) returns the nth integer k for which 6*k+5 and 6*k+7 are
 *            twin primes. The function is implemented by incrementally
 *            searching for twin‑prime pairs, storing the discovered k-values
 *            in a static cache. Subsequent calls reuse the cache and continue
 *            the search from the last checked index, achieving...
 * Range:     u8 n <= 16, u16 n <= 880, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache for discovered k-values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries stored in cache */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static idx_t last_checked_k = 0; /* last processed k (next to test) */

    /* Keep computing until we have at least n entries */
    while (cache_len < n) {
        /* compute 6*k+5 and 6*k+7 using seq_t arithmetic */
        seq_t val5 = (seq_t)last_checked_k * 6 + 5;
        seq_t val7 = (seq_t)last_checked_k * 6 + 7;

        /* If both are prime, store k in cache */
        if (is_prime_small(val5) && is_prime_small(val7)) {
            /* expand cache if needed */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* On allocation failure, abort (cannot recover) */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked_k;
        }
        /* advance to next candidate k */
        last_checked_k++;
    }
    /* Return the n-th entry (1‑based index) */
    return cache[n - 1];
}
