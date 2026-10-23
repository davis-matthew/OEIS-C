/*
 * A045452  Primes congruent to {4, 5} mod 7.
 * https://oeis.org/A045452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   compute the nth prime congruent to 4 or 5 modulo 7
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=5, a(2)=11, a(3)=19, a(4)=47, … */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;          /* array of a(k) */
    static size_t cache_capacity = 0;    /* allocated size of cache */
    static size_t found = 0;             /* number of terms already computed */
    static uint64_t next_candidate = 2;  /* next integer to test for primality */

    if (n == 0) return 0;   /* undefined for n<1, but harmless guard */

    /* if the requested term is already cached, return it */
    if ((size_t)n <= found)
        return cache[n - 1];

    /* grow the cache if necessary */
    if (cache_capacity < (size_t)n) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 8;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* generate new terms until we reach the nth one */
    while (found < n) {
        ++next_candidate;                                 /* test next integer */
        if (is_prime((idx_t)next_candidate)) {
            uint64_t mod = next_candidate % 7;
            if (mod == 4 || mod == 5) {
                ++found;
                cache[found - 1] = (seq_t)next_candidate;
            }
        }
    }

    return cache[n - 1];
}
