/*
 * A142874  Primes congruent to 33 mod 62.
 * https://oeis.org/A142874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime ≡ 33 (mod 62)
 * Range:     u8 n <= 1, u16 n <= 222, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

extern seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset is 1, value for n==0 irrelevant */

    /* static cache for found terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* static pointer to the next odd candidate for primality testing */
    static idx_t next = 3;                /* first odd > 2 */

    /* extend cache until it contains the requested term */
    while (cache_len < (size_t)n) {
        /* find the next prime */
        idx_t candidate = next;
        for (;;) {
            if (is_prime(candidate)) {          /* prime found */
                if (candidate % 62 == 33) {      /* meet congruence condition */
                    if (cache_len >= cache_cap) {
                        cache_cap = cache_cap ? cache_cap << 1 : 16;
                        seq_t *tmp = (seq_t *)realloc(cache,
                                                       cache_cap * sizeof(seq_t));
                        if (!tmp) {
                            exit(EXIT_FAILURE);    /* out of memory */
                        }
                        cache = tmp;
                    }
                    cache[cache_len++] = (seq_t)candidate;
                }
                next = candidate + 2;            /* skip to next odd candidate */
                break;
            }
            candidate += 2;                     /* test only odd numbers */
        }
    }

    return cache[(size_t)n - 1];
}
