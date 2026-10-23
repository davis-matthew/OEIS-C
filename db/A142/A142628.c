/*
 * A142628  Primes congruent to 38 mod 55.
 * https://oeis.org/A142628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 38 (mod 55). *
 *            Implementation: iterate over numbers of the form 38 + 55*k, *
 *            test each for primality with the supplied is_prime() routine, *
 *            and cache the primes that satisfy the congruence. The function *
 *            works for any order of calls
 * Range:     u8 none, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes in the residue class 38 mod 55. */
    static seq_t  *cache   = NULL;
    static size_t cache_sz = 0;   /* number of primes cached so far */
    static size_t cache_cap = 0;  /* current capacity of cache      */
    static idx_t  next_k   = 0;   /* next k to test (candidate = 38 + 55*k) */

    if (n == 0)                /* offset starts at 1; return 0 for safety */
        return (seq_t)0;

    /* Grow the cache until we have at least n primes. */
    while (cache_sz < n) {
        idx_t cand = 38 + 55 * next_k;   /* candidate number */

        if (is_prime(cand)) {
            /* Expand cache if needed. */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
