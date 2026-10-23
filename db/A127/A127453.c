/*
 * A127453  Primes p such that 6p - 13 and 6p + 13 are also primes.
 * https://oeis.org/A127453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that 6p-13 and 6p+13 are also
 *            primes
 * Range:     u8 n <= 12, u16 n <= 350, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored                          */
    static size_t cache_cap = 0;   /* current capacity of cache                       */
    static idx_t last_checked = 1;/* last integer examined for the property          */

    /* Return from cache if possible. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache until it contains the n-th term. */
    while (cache_len < n) {
        /* Find the next integer to test. */
        do {
            ++last_checked;
        } while (last_checked < 3);          /* only consider p >= 3            */

        /* Test whether last_checked is a prime with the required property. */
        if (is_prime(last_checked)) {
            idx_t t1 = last_checked * 6 - 13;
            idx_t t2 = last_checked * 6 + 13;
            if (is_prime(t1) && is_prime(t2)) {
                /* Append to cache. */
                if (cache_len == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)last_checked;
            }
        }
    }
    return cache[n - 1];
}
