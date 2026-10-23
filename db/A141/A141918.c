/*
 * A141918  Primes congruent to 14 mod 23.
 * https://oeis.org/A141918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 270, u32 n <= 272, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A141918: primes congruent to 14 modulo 23.
 * Computation: incremental search for primes of the form 14 + 23*k
 * using the provided is_prime_big routine.  Results are memoised.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cache of found terms */
    static size_t cache_count = 0;     /* number of cached terms */
    static size_t cache_cap   = 0;     /* allocated capacity of cache */

    static seq_t candidate = 14;       /* last tested candidate */

    /* initial allocation of cache */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* extend cache until we have the requested term */
    while (cache_count < n) {
        /* next candidate with the required residue */
        candidate += 23;

        /* primality test using the built‑in helper */
        if (is_prime_big(candidate)) {
            /* ensure there is room */
            if (cache_count == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_count++] = candidate;
        }
    }

    /* return the n‑th term (1‑based index) */
    return cache[n - 1];
}
