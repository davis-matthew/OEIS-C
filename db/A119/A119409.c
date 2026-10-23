/*
 * A119409  Numbers k such that 235*k + 1 is prime.
 * https://oeis.org/A119409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = the n-th integer k such that 235*k + 1 is prime
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 33, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of found k values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    /* Next integer to test for the property */
    static idx_t next_candidate = 1;

    if (n == 0) {
        /* Offset is 1; return 0 in a graceful fallback */
        return (seq_t)0;
    }

    /* Return cached value if already known */
    if (n <= cache_size) {
        return cache[(size_t)(n - 1)];
    }

    /* Initial allocation of the cache if necessary */
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {
            abort(); /* allocation failure */
        }
    }

    /* Generate k values until we have at least n in the cache */
    while (cache_size < n) {
        seq_t k = (seq_t)next_candidate++;
        seq_t m = (seq_t)235 * k + (seq_t)1;

        if (is_prime_big(m)) {
            /* Ensure enough space in the cache */
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    abort();
                }
                cache = tmp;
            }
            cache[cache_size++] = k;
        }
    }

    return cache[(size_t)(n - 1)];
}
