/*
 * A082496  Numbers of the form 2p+1, where p and p+2 are a pair of twin
 *          primes.
 * https://oeis.org/A082496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*p_n + 1 where p_n is the n-th first member of a twin
 *            prime pair (p and p+2 are both prime). The function builds the
 *            sequence lazily and caches the results. It works for any seq_t
 *            and idx_t as defined by the build
 * Range:     u8 n <= 10, u16 n <= 505, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_p     = 0;   /* last p found (first of twin pair) */

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)7;                  /* 2*3 + 1 */
        cache_len = 1;
        last_p = 3;
    }

    while (cache_len < n) {
        idx_t p = last_p + 2;                 /* next odd candidate */

        /* search for next twin prime pair */
        while (1) {
            if (is_prime(p) && is_prime(p + 2)) {
                seq_t value = (seq_t)2 * p + 1;

                /* grow cache if necessary */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
                    if (!tmp) { cache_cap = cache_len; break; }
                    cache = tmp;
                }

                cache[cache_len++] = value;
                last_p = p;
                break;
            }
            p += 2;
        }
    }

    return cache[n - 1];
}
