/*
 * A095081  Fibodd primes, i.e., primes p whose Zeckendorf-expansion
 *          A014417(p) ends with one.
 * https://oeis.org/A095081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2530, u32 n <= 1797, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zeckendorf_ending_one, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A095081: primes p whose Zeckendorf expansion A014417(p) ends with 1.
   The function computes primes by a sieve-like search and caches the
   results so that any n ≥ 1 can be answered quickly. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */
#include "../utils/has_zeckendorf_ending_one.h"

/* --------------------------------------------------------------------- */
/* Helper: Zeckendorf representation test                                   */

/* --------------------------------------------------------------------- */
/* Main sequence function                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; undefined for n = 0 */

    /* Cache of found Fibodd primes. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored primes */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failed – return 0 */
    }

    /* Generate new primes until we have at least n entries. */
    while (cache_len < n) {
        seq_t candidate = 2;
        if (cache_len > 0) candidate = cache[cache_len - 1] + 1;  /* start after last prime */

        /* Search for the next Fibodd prime. */
        for (;;) {
            if (candidate < 2) candidate = 2;  /* safety */

            if (is_prime_big(candidate) && has_zeckendorf_ending_one(candidate)) {
                /* Store the new prime. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;  /* allocation failed – return 0 */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
                break;   /* move on to generate the next prime if needed */
            }
            candidate = candidate + 1;
        }
    }

    return cache[n - 1];
}
