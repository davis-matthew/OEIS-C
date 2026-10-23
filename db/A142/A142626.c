/*
 * A142626  Primes congruent to 36 mod 55.
 * https://oeis.org/A142626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142626: primes congruent to 36 modulo 55
   (search based on is_prime, stored in a static cache). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

/* a(n) returns the n-th prime p such that p % 55 == 36.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n=0 is undefined */

    static seq_t *cache = NULL;    /* cache of found primes */
    static size_t cache_len = 0;   /* number of primes stored */
    static size_t cache_cap = 0;   /* capacity of cache */
    static seq_t next_candidate = 36;   /* last tested value */

    while (cache_len < n) {
        /* jump to the next number ≡ 36 (mod 55) after the previous prime */
        next_candidate += 55;
        /* find the next prime with that residue */
        while (!is_prime((idx_t)next_candidate))
            next_candidate += 55;

        /* grow cache if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) return 0;          /* allocation failure: return 0 */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = next_candidate;
    }

    return cache[n - 1];
}
