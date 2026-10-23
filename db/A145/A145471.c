/*
 * A145471  Primes p such that (5+p)/2 is prime.
 * https://oeis.org/A145471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 637, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145471: a(n) = 2*A063909(n)-5, primes p such that (5+p)/2 is prime */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Static cache of already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_checked = 3;   /* last odd number examined */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < n) {
        idx_t p = last_checked + 2;   /* next odd candidate */

        while (cache_len < n) {
            if (is_prime(p)) {
                /* compute q = (p+5)/2 and test its primality */
                seq_t q = (seq_t)p + 5;
                q /= 2;
                if (is_prime((idx_t)q)) {
                    if (cache_len == cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 16;
                        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)p;
                }
            }
            last_checked = p;
            p += 2;
        }
    }

    return cache[n - 1];
}
