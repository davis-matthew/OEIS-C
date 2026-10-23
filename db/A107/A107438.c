/*
 * A107438  Primes p such that 7*p+2 or 2*p+7 is prime.
 * https://oeis.org/A107438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that 7*p+2 OR 2*p+7 is prime
 * Range:     u8 n <= 22, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static inline bool ok_for_prime(uint64_t p)
{
    /* Test whether 7*p+2 or 2*p+7 is prime. */
    uint64_t a = (uint64_t)((unsigned __int128)7 * p + 2);
    uint64_t b = (uint64_t)((unsigned __int128)2 * p + 7);
    return is_prime(a) || is_prime(b);
}

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0, safe fallback */

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Static variable tracking the current candidate integer. */
    static uint64_t cand = 2;

    while (cache_len < (size_t)n) {
        /* Scan candidates until we find a suitable prime. */
        while (true) {
            if (is_prime(cand) && ok_for_prime(cand)) {
                /* Store found term. */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)cand;
                cand++;                /* move past this prime */
                break;                 /* proceed to next required term */
            }
            cand++;
        }
    }

    return cache[n - 1];
}
