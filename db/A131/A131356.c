/*
 * A131356  Numbers k such that p1 = 10k+9 and p2 = p1+2 are twin primes.
 * https://oeis.org/A131356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all integers k such that both 
   10k+9 and 10k+11 are prime (twin primes).  The function returns
   the n‑th such k, with a(1)=2 (the first member of the sequence). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides:  bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    if (n == 0) {                 /* offset starts at 1 – value for a(0) undefined */
        return 0;
    }

    /* static cache of previously found k values */
    static seq_t *cache      = NULL;
    static size_t cache_sz  = 0;
    static size_t cache_cap = 0;
    static idx_t  last_k     = 0;   /* last k that was examined */

    /* Grow the cache until it contains at least n elements */
    while (cache_sz < (size_t)n) {
        idx_t k = last_k + 1;          /* candidate k */

        /* candidate twin primes: 10k+9 and 10k+11 */
        idx_t p1 = 10 * k + 9;
        idx_t p2 = p1 + 2;

        if (is_prime(p1) && is_prime(p2)) {
            /* allocate more space if needed */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!tmp) {            /* out‑of‑memory – abort safely */
                    if (cache) free(cache);
                    cache = NULL;
                    cache_sz = cache_cap = 0;
                    return 0;
                }
                if (cache) {
                    memcpy(tmp, cache, cache_sz * sizeof(seq_t));
                    free(cache);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)k;   /* store found k */
        }

        last_k = k;
    }

    return cache[(size_t)n - 1];
}
