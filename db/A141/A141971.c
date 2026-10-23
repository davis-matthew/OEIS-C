/*
 * A141971  Primes congruent to 15 mod 28.
 * https://oeis.org/A141971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 15 (mod 28)
 * Range:     u8 n <= 5, u16 n <= 546, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 – undefined for n < 1 */

    /* static cache of the first primes ≡ 15 (mod 28) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  next_candidate = 2;    /* smallest number to test next */

    /* we keep generating until we have at least n terms */
    while (cache_size < (size_t)n) {
        /* pull the next prime */
        idx_t p = next_candidate;
        while (!is_prime(p)) ++p;          /* find next prime */

        if ((p % 28ULL) == 15ULL) {       /* congruent to 15 mod 28 */
            if (cache_size >= cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure – give up */
                    if (cache) free(cache);
                    cache = NULL; cache_size = 0; cache_cap = 0;
                    return 0;
                }
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)p;
        }

        next_candidate = p + 1;  /* start next search after the prime found */
    }

    return cache[(size_t)n - 1];
}
