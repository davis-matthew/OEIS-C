/*
 * A045323  Primes congruent to {1, 2, 3, 7} (mod 8).
 * https://oeis.org/A045323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 4899, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime p such that p % 8 ∈ {1, 2, 3, 7} */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 */

    /* static cache for the primes that meet the condition */
    static seq_t *cache      = NULL;   /* array of found primes          */
    static size_t cache_cap = 0;       /* allocated capacity             */
    static idx_t  cache_len  = 0;       /* number of primes stored        */
    static uint64_t last_checked = 1;   /* greatest candidate checked so far */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Find the next candidate prime. */
        last_checked++;
        if (last_checked == 2 || (last_checked & 1U)) {  /* skip even > 2 */
            if (is_prime_small(last_checked)) {
                /* Accept if p ≡ 1,2,3,7 (mod 8).  2 (the only even prime) is fine. */
                if (last_checked == 2 || ((last_checked & 7U) != 5U)) {
                    /* Ensure capacity. */
                    if (cache_len >= (idx_t)cache_cap) {
                        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) {
                            /* Allocation failure – return 0 to signal error. */
                            free(cache);
                            cache = NULL;
                            cache_cap = cache_len = 0;
                            return (seq_t)0;
                        }
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = (seq_t)last_checked;
                }
            }
        }
    }

    return cache[n - 1];
}
