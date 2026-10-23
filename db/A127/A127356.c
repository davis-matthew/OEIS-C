/*
 * A127356  a(n) is the smallest k > 0 such that k^2 + prime(n) is prime.
 * https://oeis.org/A127356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k > 0 such that k^2 + prime(n) is prime
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache for already computed values.  Using zero as
       "not yet computed" because a(n) is always > 0. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure the cache can accommodate index n (1‑based). */
    if (n == 0) return 0;                /* undefined for the offset, safe guard */
    if (cache == NULL || cache_cap < n) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
        while (new_cap < n) new_cap <<= 1;   /* double until large enough */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;   /* allocation failure */
        /* Zero-initialize the newly added part of the array. */
        if (new_cache != cache) {
            memmove(new_cache, cache, cache_cap * sizeof(seq_t));
            memset(new_cache + cache_cap, 0,
                   (new_cap - cache_cap) * sizeof(seq_t));
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If already computed, just return it. */
    if (cache[n - 1] != 0) return cache[n - 1];

    /* Compute a(n) on demand. */
    seq_t prime_n = (seq_t)get_prime(n);  /* nth prime, n >= 1 */
    seq_t k = 1;
    for (;;) {
        seq_t val = k * k + prime_n;
        if (is_prime_big(val)) {
            cache[n - 1] = k;
            break;
        }
        k++;
    }

    return cache[n - 1];
}
