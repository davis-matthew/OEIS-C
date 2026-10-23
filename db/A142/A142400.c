/*
 * A142400  Primes congruent to 7 mod 48.
 * https://oeis.org/A142400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 7 modulo 48
 * Range:     u8 n <= 4, u16 n <= 418, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incremental search with static cache.  Works for any order of calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1, guard for 0 */

    /* static cache of primes congruent to 7 (mod 48) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;        /* already found primes */
    static size_t cache_cap  = 0;        /* allocated capacity */
    static idx_t candidate = 7;          /* current candidate (48k+7) */

    /* enlarge cache if needed */
    if (cache_size < (size_t)n) {
        size_t needed = (size_t)n;
        if (cache_cap < needed) {
            size_t new_cap = (cache_cap == 0) ? needed : cache_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;   /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* find additional primes until we have at least n of them */
        while (cache_size < (size_t)n) {
            if (is_prime(candidate))
                cache[cache_size++] = (seq_t)candidate;
            candidate += 48;
        }
    }

    return cache[n - 1];
}
